#!/usr/bin/env python3
"""Shared performance-measurement utilities for the SIFT/FLANN ROS2 pipeline.

Provides a PerfTracker class that:
  * Times named stages with high-resolution counters (time.perf_counter)
  * Records per-stage metadata (counts, sizes, throughput ...)
  * Samples process CPU / memory usage (via /proc, no extra deps)
  * Prints a detailed, aligned report to the terminal (via a logger)
  * Persists the same report as a .txt file and a machine-readable .json file
"""

import json
import os
import platform
import resource
import time
from collections import OrderedDict
from contextlib import contextmanager
from datetime import datetime


def _read_proc_status():
    """Read memory info of the current process from /proc (Linux only)."""
    info = {}
    try:
        with open(f'/proc/{os.getpid()}/status', 'r') as f:
            for line in f:
                if line.startswith(('VmRSS:', 'VmHWM:', 'VmSize:', 'Threads:')):
                    key, value = line.split(':', 1)
                    info[key] = value.strip()
    except OSError:
        pass
    return info


class PerfTracker:
    """Collects detailed per-stage performance metrics for a node."""

    def __init__(self, node_name, output_dir='output'):
        self.node_name = node_name
        self.output_dir = output_dir
        self.start_wall = datetime.now()
        self.start_perf = time.perf_counter()
        self.start_cpu = time.process_time()
        self.stages = OrderedDict()   # stage_name -> dict of metrics
        self.events = OrderedDict()   # event_name -> timestamp ms since start
        self.extra = OrderedDict()    # any extra key/value info
        os.makedirs(self.output_dir, exist_ok=True)

    # ── Timing API ───────────────────────────────────────────────────
    @contextmanager
    def stage(self, name, **meta):
        """Context manager that times a stage and records wall + CPU time."""
        t0_wall = time.perf_counter()
        t0_cpu = time.process_time()
        record = {'meta': dict(meta)}
        try:
            yield record
        finally:
            wall_ms = (time.perf_counter() - t0_wall) * 1000.0
            cpu_ms = (time.process_time() - t0_cpu) * 1000.0
            record['wall_ms'] = wall_ms
            record['cpu_ms'] = cpu_ms
            record['started_at_ms'] = (t0_wall - self.start_perf) * 1000.0
            self.stages[name] = record

    def mark_event(self, name):
        """Record a point-in-time event (ms since tracker creation)."""
        self.events[name] = (time.perf_counter() - self.start_perf) * 1000.0
        return self.events[name]

    def add_info(self, key, value):
        self.extra[key] = value

    def add_stage_meta(self, stage_name, key, value):
        if stage_name in self.stages:
            self.stages[stage_name]['meta'][key] = value

    # ── System / resource snapshot ───────────────────────────────────
    def _resource_snapshot(self):
        ru = resource.getrusage(resource.RUSAGE_SELF)
        snap = {
            'peak_rss_mb': ru.ru_maxrss / 1024.0,           # KB -> MB on Linux
            'user_cpu_s': ru.ru_utime,
            'system_cpu_s': ru.ru_stime,
            'voluntary_ctx_switches': ru.ru_nvcsw,
            'involuntary_ctx_switches': ru.ru_nivcsw,
            'page_faults_minor': ru.ru_minflt,
            'page_faults_major': ru.ru_majflt,
        }
        snap.update({f'proc_{k.lower()}': v for k, v in _read_proc_status().items()})
        return snap

    # ── Reporting ────────────────────────────────────────────────────
    def build_report_lines(self):
        """Return the full human-readable report as a list of lines."""
        total_wall_ms = (time.perf_counter() - self.start_perf) * 1000.0
        total_cpu_ms = (time.process_time() - self.start_cpu) * 1000.0
        res = self._resource_snapshot()

        W = 78
        bar = '=' * W
        thin = '-' * W
        lines = []
        lines.append(bar)
        lines.append(f'  PERFORMANCE REPORT  --  {self.node_name}')
        lines.append(bar)
        lines.append(f'  Date/time        : {self.start_wall.strftime("%Y-%m-%d %H:%M:%S")}')
        lines.append(f'  Host             : {platform.node()}  ({platform.machine()})')
        lines.append(f'  Python           : {platform.python_version()}')
        lines.append(f'  PID              : {os.getpid()}')
        if self.extra:
            lines.append(thin)
            lines.append('  GENERAL INFO')
            lines.append(thin)
            for k, v in self.extra.items():
                lines.append(f'  {k:<32s}: {v}')

        # Stage table
        lines.append(thin)
        lines.append('  STAGE TIMINGS')
        lines.append(thin)
        lines.append(f'  {"Stage":<34s}{"Wall (ms)":>11s}{"CPU (ms)":>11s}{"% wall":>9s}')
        lines.append(thin)
        stage_total = 0.0
        for name, rec in self.stages.items():
            pct = rec['wall_ms'] / total_wall_ms * 100.0 if total_wall_ms > 0 else 0.0
            stage_total += rec['wall_ms']
            lines.append(
                f'  {name:<34s}{rec["wall_ms"]:>11.3f}{rec["cpu_ms"]:>11.3f}{pct:>8.2f}%'
            )
            for mk, mv in rec['meta'].items():
                lines.append(f'      - {mk}: {mv}')
        lines.append(thin)
        pct_sum = stage_total / total_wall_ms * 100.0 if total_wall_ms > 0 else 0.0
        lines.append(f'  {"Sum of measured stages":<34s}{stage_total:>11.3f}{"":>11s}{pct_sum:>8.2f}%')
        lines.append(f'  {"TOTAL (since node start)":<34s}{total_wall_ms:>11.3f}{total_cpu_ms:>11.3f}{100.0:>8.2f}%')

        # Events
        if self.events:
            lines.append(thin)
            lines.append('  TIMELINE EVENTS (ms since node start)')
            lines.append(thin)
            for name, ts in self.events.items():
                lines.append(f'  {name:<44s}{ts:>11.3f} ms')

        # Resources
        lines.append(thin)
        lines.append('  PROCESS RESOURCE USAGE')
        lines.append(thin)
        lines.append(f'  Peak RSS memory                   : {res["peak_rss_mb"]:.2f} MB')
        lines.append(f'  User CPU time                     : {res["user_cpu_s"]:.3f} s')
        lines.append(f'  System CPU time                   : {res["system_cpu_s"]:.3f} s')
        lines.append(f'  Context switches (vol/invol)      : '
                     f'{res["voluntary_ctx_switches"]} / {res["involuntary_ctx_switches"]}')
        lines.append(f'  Page faults (minor/major)         : '
                     f'{res["page_faults_minor"]} / {res["page_faults_major"]}')
        for k in ('proc_vmrss', 'proc_vmhwm', 'proc_vmsize', 'proc_threads'):
            if k in res:
                pretty = k.replace('proc_', '').upper()
                lines.append(f'  {pretty:<34s}: {res[k]}')
        lines.append(bar)
        return lines, total_wall_ms, total_cpu_ms, res

    def log_report(self, logger):
        """Print the full report to terminal via the node logger."""
        lines, *_ = self.build_report_lines()
        logger.info('\n' + '\n'.join(lines))

    def save_report(self, basename):
        """Save the report as <output_dir>/<basename>.txt and .json.

        Returns (txt_path, json_path).
        """
        lines, total_wall_ms, total_cpu_ms, res = self.build_report_lines()

        txt_path = os.path.join(self.output_dir, basename + '.txt')
        with open(txt_path, 'w') as f:
            f.write('\n'.join(lines) + '\n')

        payload = {
            'node': self.node_name,
            'timestamp': self.start_wall.isoformat(),
            'host': platform.node(),
            'python': platform.python_version(),
            'pid': os.getpid(),
            'total_wall_ms': total_wall_ms,
            'total_cpu_ms': total_cpu_ms,
            'info': dict(self.extra),
            'stages': {
                name: {
                    'wall_ms': rec['wall_ms'],
                    'cpu_ms': rec['cpu_ms'],
                    'started_at_ms': rec['started_at_ms'],
                    'meta': rec['meta'],
                }
                for name, rec in self.stages.items()
            },
            'events_ms': dict(self.events),
            'resources': res,
        }
        json_path = os.path.join(self.output_dir, basename + '.json')
        with open(json_path, 'w') as f:
            json.dump(payload, f, indent=2, default=str)

        return txt_path, json_path