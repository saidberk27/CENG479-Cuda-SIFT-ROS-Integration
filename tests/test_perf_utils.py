"""
Unit tests for the PerfTracker (Python) performance measurement utility.
"""

import json
import os
import tempfile
from pathlib import Path

import pytest

from sequential.perf_utils import PerfTracker


class TestPerfTracker:
    """Test suite for PerfTracker."""

    @pytest.fixture
    def tracker(self):
        """Create a PerfTracker instance in a temp directory."""
        with tempfile.TemporaryDirectory() as tmpdir:
            yield PerfTracker("test_node", output_dir=tmpdir)

    def test_initialization(self, tracker):
        """Test that PerfTracker initializes correctly."""
        assert tracker.node_name == "test_node"
        assert tracker.stages == {}
        assert tracker.events == {}
        assert tracker.extra == {}

    def test_stage_timing(self, tracker):
        """Test that a stage records wall and CPU time."""
        with tracker.stage("test_stage") as rec:
            # Simulate some work
            _ = [i**2 for i in range(10000)]

        assert "test_stage" in tracker.stages
        record = tracker.stages["test_stage"]
        assert record["wall_ms"] > 0
        assert record["cpu_ms"] > 0
        assert record["started_at_ms"] >= 0

    def test_multiple_stages(self, tracker):
        """Test that multiple stages are recorded in order."""
        with tracker.stage("stage_a"):
            pass
        with tracker.stage("stage_b"):
            pass

        stages = list(tracker.stages.keys())
        assert stages == ["stage_a", "stage_b"]

    def test_stage_metadata(self, tracker):
        """Test that per-stage metadata is recorded."""
        with tracker.stage("data_loading") as rec:
            rec["meta"]["file_size"] = "42 MB"
            rec["meta"]["num_items"] = "1000"

        meta = tracker.stages["data_loading"]["meta"]
        assert meta["file_size"] == "42 MB"
        assert meta["num_items"] == "1000"

    def test_mark_event(self, tracker):
        """Test event marking."""
        ts = tracker.mark_event("started")
        assert "started" in tracker.events
        assert tracker.events["started"] == ts
        assert ts >= 0

    def test_add_info(self, tracker):
        """Test adding extra info."""
        tracker.add_info("key1", "value1")
        tracker.add_info("key2", "value2")
        assert tracker.extra["key1"] == "value1"
        assert tracker.extra["key2"] == "value2"

    def test_add_stage_meta_after_stage(self, tracker):
        """Test updating stage metadata after the stage completes."""
        with tracker.stage("process"):
            pass

        tracker.add_stage_meta("process", "result", "success")
        assert tracker.stages["process"]["meta"]["result"] == "success"

    def test_empty_stage_meta(self, tracker):
        """Test metadata on stage with no additional info."""
        with tracker.stage("empty_stage"):
            pass
        assert tracker.stages["empty_stage"]["meta"] == {}

    def test_save_report_creates_files(self, tracker):
        """Test that save_report creates .txt and .json files."""
        with tracker.stage("init"):
            pass
        tracker.add_info("version", "1.0")

        txt_path, json_path = tracker.save_report("test_report")

        assert os.path.exists(txt_path)
        assert os.path.exists(json_path)

        # Verify JSON content
        with open(json_path) as f:
            data = json.load(f)
        assert data["node"] == "test_node"
        assert "stages" in data
        assert "init" in data["stages"]

    def test_report_contains_timing_info(self, tracker):
        """Test that the report contains stage timing details."""
        with tracker.stage("compute") as rec:
            _ = sum(range(100000))

        _, json_path = tracker.save_report("timing_test")
        with open(json_path) as f:
            data = json.load(f)

        stage = data["stages"]["compute"]
        assert stage["wall_ms"] > 0
        assert stage["cpu_ms"] > 0

    def test_build_report_lines(self, tracker):
        """Test that build_report_lines returns non-empty output."""
        with tracker.stage("test_stage"):
            pass
        lines, *_ = tracker.build_report_lines()
        assert len(lines) > 0
        assert any("PERFORMANCE REPORT" in line for line in lines)
        assert any("test_node" in line for line in lines)

    def test_order_of_stages_preserved(self, tracker):
        """Test that stages maintain their insertion order."""
        with tracker.stage("first"):
            pass
        with tracker.stage("second"):
            pass
        with tracker.stage("third"):
            pass

        assert list(tracker.stages.keys()) == ["first", "second", "third"]

    def test_concurrent_stage_order(self, tracker):
        """Test nested stages (though not recommended, should not crash)."""
        with tracker.stage("outer"):
            with tracker.stage("inner"):
                pass

        assert "outer" in tracker.stages
        assert "inner" in tracker.stages

    def test_report_not_crash_with_no_stages(self, tracker):
        """Test that building a report with no stages doesn't crash."""
        lines, *_ = tracker.build_report_lines()
        assert len(lines) > 0