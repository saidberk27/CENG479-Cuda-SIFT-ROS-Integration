#!/usr/bin/env python3
"""
benchmark.py  —  SIFT-FLANN Pipeline Benchmark
================================================
Runs both the sequential (Python/CPU) and paralel (C++/CUDA) pipelines,
reads their pipeline_performance.json outputs, and prints a detailed
speedup comparison table.

Usage:
    cd /home/sberk/Desktop/paralel
    source install/setup.bash
    python3 benchmark.py [--large ankara.jpg] [--query test.jpg]
"""

import argparse
import json
import os
import subprocess
import sys
import time

# ── Paths ─────────────────────────────────────────────────────────────────────
WORKSPACE   = os.path.dirname(os.path.abspath(__file__))
SEQ_OUT     = os.path.join(WORKSPACE, 'src', 'sequential', 'output', 'pipeline_performance.json')
PAR_OUT     = os.path.join(WORKSPACE, 'src', 'paralel',   'output', 'pipeline_performance.json')
SETUP_BASH  = os.path.join(WORKSPACE, 'install', 'setup.bash')

W = 82   # report width


def banner(title):
    print('=' * W)
    print(f'  {title}')
    print('=' * W)


def section(title):
    print('-' * W)
    print(f'  {title}')
    print('-' * W)


def run_pipeline(name, ros2_cmd, timeout=300):
    """Run a ROS2 command inside a bash shell that sources the workspace."""
    print(f'\n{"─"*W}')
    print(f'  ▶  Running {name} pipeline …')
    print(f'{"─"*W}')
    cmd = f'source {SETUP_BASH} && {ros2_cmd}'
    t0 = time.perf_counter()
    result = subprocess.run(
        ['bash', '-c', cmd],
        cwd=WORKSPACE,
        timeout=timeout,
    )
    elapsed = (time.perf_counter() - t0) * 1000.0
    if result.returncode != 0:
        print(f'  ✗  {name} pipeline exited with code {result.returncode}')
        sys.exit(1)
    print(f'  ✓  {name} finished in {elapsed:.0f} ms (wall, including ROS2 startup)')
    return elapsed


def load_json(path, label):
    if not os.path.exists(path):
        print(f'  ERROR: {label} report not found at {path}')
        sys.exit(1)
    with open(path) as f:
        return json.load(f)


def fmt_ms(v):
    return f'{v:>10.1f} ms'


def speedup(seq_ms, par_ms):
    if par_ms <= 0:
        return float('inf')
    return seq_ms / par_ms


def print_comparison(seq, par):
    """Print the side-by-side comparison table."""

    # ── Header ────────────────────────────────────────────────────────────────
    banner('SIFT-FLANN PIPELINE BENCHMARK  —  Sequential vs Paralel (CUDA)')
    print(f'  Sequential : {seq.get("node","?")}  '
          f'({seq.get("language","Python")})')
    print(f'  Paralel    : {par.get("node","?")}  '
          f'({par.get("language","C++/CUDA")})')
    print(f'  Host       : {par.get("host","?")}')
    print(f'  Timestamp  : {par.get("timestamp","?")}')

    # ── General info ──────────────────────────────────────────────────────────
    section('GENERAL INFO')
    seq_info = seq.get('info', {})
    par_info = par.get('info', {})
    keys = ['large_image_path', 'query_image_path', 'large_keypoints',
            'query_keypoints', 'good_matches_after_ratio_test',
            'ransac_inliers', 'pipeline_success']
    for k in keys:
        sv = seq_info.get(k, seq_info.get(k.replace('_', ' '), '—'))
        pv = par_info.get(k, '—')
        print(f'  {k:<36s}  seq: {str(sv):<12s}  par: {str(pv)}')

    # ── Total wall time ────────────────────────────────────────────────────────
    section('TOTAL WALL TIME')
    # Sequential: use subscriber total (it waits for publisher, so it covers E2E)
    seq_total = seq.get('subscriber_total_wall_ms') or seq.get('total_wall_ms', 0.0)
    par_total = par.get('total_wall_ms', 0.0)
    sp_total  = speedup(seq_total, par_total)
    print(f'  {"Metric":<38s}  {"Sequential":>12s}  {"Paralel":>12s}  {"Speedup":>10s}')
    print(f'  {"-"*76}')
    print(f'  {"Subscriber/pipeline total wall":<38s}  {fmt_ms(seq_total)}  '
          f'{fmt_ms(par_total)}  {sp_total:>9.2f}×')
    pub_total = seq.get('publisher_total_wall_ms', 0.0)
    print(f'  {"Publisher total wall (seq only)":<38s}  {fmt_ms(pub_total)}  '
          f'{"—":>12s}  {"—":>10s}')

    # ── Stage-level comparison ─────────────────────────────────────────────────
    section('STAGE-LEVEL COMPARISON  (wall ms)')
    print(f'  {"Stage":<42s}  {"Sequential":>12s}  {"Paralel":>12s}  {"Speedup":>10s}')
    print(f'  {"-"*80}')

    seq_stages = seq.get('stages', {})
    par_stages = par.get('stages', {})

    # Map comparable stages
    stage_map = [
        # (label, seq_stage_key, par_stage_key)
        ('Image loading',
         'pub_1_image_loading',
         None),
        ('Grayscale conversion',
         'pub_2_grayscale_conversion',
         None),
        ('SIFT large image',
         'pub_3_sift_large_image',
         None),
        ('SIFT query image',
         'pub_4_sift_query_image',
         None),
        ('Message serialization (seq)',
         'pub_5_message_serialization',
         None),
        ('Publish / VRAM upload+publish',
         'pub_6_publish_messages',
         'A_parallel_sift_upload_publish'),
        ('Subscribe / VRAM bus hand-off',
         None,
         'B_vram_bus_subscribe'),
        ('Data reconstruction (seq)',
         'sub_1_data_reconstruction',
         None),
        ('kNN matching',
         'sub_3_flann_knn_matching',
         'C_gpu_knn_match_ratio'),
        ("Lowe's ratio test",
         'sub_4_lowes_ratio_test',
         None),
        ('Point extraction',
         'sub_5_point_extraction',
         'D_collect_matched_points'),
        ('Homography RANSAC',
         'sub_6_homography_ransac',
         'E_homography_ransac'),
        ('Perspective transform',
         'sub_7_perspective_transform',
         'F_perspective_transform'),
    ]

    for label, sk, pk in stage_map:
        sv = seq_stages.get(sk, {}).get('wall_ms') if sk else None
        pv = par_stages.get(pk, {}).get('wall_ms') if pk else None
        sv_str = fmt_ms(sv) if sv is not None else f'{"—":>12s}'
        pv_str = fmt_ms(pv) if pv is not None else f'{"—":>12s}'
        if sv is not None and pv is not None:
            sp_str = f'{speedup(sv, pv):>9.2f}×'
        elif sv is not None:
            sp_str = f'{"(no par)":>10s}'
        elif pv is not None:
            sp_str = f'{"(no seq)":>10s}'
        else:
            sp_str = f'{"—":>10s}'
        print(f'  {label:<42s}  {sv_str}  {pv_str}  {sp_str}')

    # ── Key speedup summary ────────────────────────────────────────────────────
    section('KEY SPEEDUP SUMMARY')

    def stage_ms(stages, key):
        return stages.get(key, {}).get('wall_ms', None)

    knn_seq = stage_ms(seq_stages, 'sub_3_flann_knn_matching')
    knn_par = stage_ms(par_stages, 'C_gpu_knn_match_ratio')
    bus_par = stage_ms(par_stages, 'B_vram_bus_subscribe')

    rows = [
        ('Total pipeline wall time',   seq_total, par_total),
    ]
    if knn_seq and knn_par:
        rows.append(('kNN matching only', knn_seq, knn_par))

    print(f'  {"Metric":<38s}  {"Sequential":>12s}  {"Paralel":>12s}  {"Speedup":>10s}')
    print(f'  {"-"*76}')
    for label, sv, pv in rows:
        print(f'  {label:<38s}  {fmt_ms(sv)}  {fmt_ms(pv)}  {speedup(sv,pv):>9.2f}×')

    if bus_par is not None:
        print(f'\n  VRAM bus hand-off latency (paralel) : {bus_par:.4f} ms  '
              f'(vs socket+DDS serialization in sequential)')

    # ── Transport overhead ─────────────────────────────────────────────────────
    section('TRANSPORT OVERHEAD')
    ser_ms  = stage_ms(seq_stages, 'pub_5_message_serialization')
    pub_ms  = stage_ms(seq_stages, 'pub_6_publish_messages')
    rec_ms  = stage_ms(seq_stages, 'sub_1_data_reconstruction')
    seq_transport = (ser_ms or 0) + (pub_ms or 0) + (rec_ms or 0)
    par_transport = bus_par or 0.0
    print(f'  Sequential transport overhead')
    print(f'    serialization + publish + reconstruction : {seq_transport:.1f} ms')
    print(f'  Paralel transport overhead')
    print(f'    VRAM bus pointer hand-off                : {par_transport:.4f} ms')
    if seq_transport > 0 and par_transport > 0:
        print(f'  Transport speedup                        : {speedup(seq_transport, par_transport):.0f}×')

    print('=' * W)
    print()


def main():
    ap = argparse.ArgumentParser(description='Benchmark sequential vs paralel SIFT pipeline')
    ap.add_argument('--large', default='ankara.jpg', help='Large/train image path')
    ap.add_argument('--query', default='test.jpg',   help='Query image path')
    ap.add_argument('--skip-run', action='store_true',
                    help='Skip running pipelines; just compare existing JSON reports')
    args = ap.parse_args()

    if not args.skip_run:
        # ── Run sequential ─────────────────────────────────────────────────────
        run_pipeline(
            'sequential',
            f'ros2 launch sequential sift_matching.launch.py '
            f'large_image:={args.large} query_image:={args.query}',
            timeout=300,
        )

        # ── Run paralel ────────────────────────────────────────────────────────
        run_pipeline(
            'paralel (CUDA)',
            f'ros2 run paralel sift_pipeline '
            f'--ros-args -p large_image:={args.large} -p query_image:={args.query}',
            timeout=300,
        )
    else:
        print('  --skip-run: using existing JSON reports')

    # ── Load reports ───────────────────────────────────────────────────────────
    seq = load_json(SEQ_OUT, 'sequential')
    par = load_json(PAR_OUT, 'paralel')

    # ── Print comparison ───────────────────────────────────────────────────────
    print_comparison(seq, par)

    # ── Save combined report ───────────────────────────────────────────────────
    combined = {
        'sequential': seq,
        'paralel': par,
        'speedup_total': speedup(
            seq.get('subscriber_total_wall_ms') or seq.get('total_wall_ms', 0),
            par.get('total_wall_ms', 0)
        ),
        'speedup_knn': speedup(
            seq.get('stages', {}).get('sub_3_flann_knn_matching', {}).get('wall_ms', 0),
            par.get('stages', {}).get('C_gpu_knn_match_ratio', {}).get('wall_ms', 0)
        ),
    }
    out_path = os.path.join(WORKSPACE, 'benchmark_results.json')
    with open(out_path, 'w') as f:
        json.dump(combined, f, indent=2, default=str)
    print(f'  Combined benchmark report saved to {out_path}')


if __name__ == '__main__':
    main()
