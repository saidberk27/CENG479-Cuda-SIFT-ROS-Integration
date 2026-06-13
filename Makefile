.PHONY: install test lint format clean build run-seq run-par benchmark help

# ──────────────────────────────────────────────────────────────────────────────
# Convenience commands for the SIFT-FLANN Pipeline project
# ──────────────────────────────────────────────────────────────────────────────

UV := $(shell command -v uv 2>/dev/null || echo "pip")
PYTHON := $(shell command -v python3 2>/dev/null || echo "python")

help: ## Show this help message
	@grep -E '^[a-zA-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | sort | \
	awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-20s\033[0m %s\n", $$1, $$2}'

# ── Installation ──────────────────────────────────────────────────────────────

install: ## Install Python dependencies (via uv or pip)
ifeq ($(UV), uv)
	uv sync --all-extras
	uv pip install -e .
else
	pip install -e ".[dev,benchmark]"
	pip install -r requirements.txt
endif

install-dev: ## Install development dependencies
ifeq ($(UV), uv)
	uv sync --extra dev
else
	pip install -e ".[dev]"
endif

# ── Linting & Formatting ──────────────────────────────────────────────────────

lint: ## Run ruff linter on Python source
	ruff check src/ scripts/ tests/

format: ## Run ruff formatter
	ruff format src/ scripts/ tests/

typecheck: ## Run mypy type checker
	mypy src/ scripts/

check: lint typecheck ## Run all checks (lint + typecheck)

# ── Testing ──────────────────────────────────────────────────────────────────

test: ## Run pytest test suite
	pytest tests/ -v --cov=src --cov-report=term-missing

test-quick: ## Run tests without coverage
	pytest tests/ -v

# ── Build & Clean ─────────────────────────────────────────────────────────────

clean: ## Clean build artifacts and caches
	find . -type d -name "__pycache__" -exec rm -rf {} + 2>/dev/null || true
	find . -type d -name ".pytest_cache" -exec rm -rf {} + 2>/dev/null || true
	find . -type d -name ".ruff_cache" -exec rm -rf {} + 2>/dev/null || true
	find . -type d -name "*.egg-info" -exec rm -rf {} + 2>/dev/null || true
	rm -rf build/ install/ log/ .mypy_cache/ dist/ *.egg
	@echo "  Cleaned build artifacts"

clean-all: clean ## Full clean including output images
	rm -rf matching_result.png benchmark_results.json
	find src -path "*/output/*.png" -delete
	find src -path "*/output/*.json" -delete
	find src -path "*/output/*.txt" -delete

# ── ROS2 Pipeline Commands ────────────────────────────────────────────────────

build-ros2: ## Build ROS2 packages with colcon
	@echo "  Building ROS2 packages..."
	colcon build --packages-up-to parallel sequential sift_interfaces
	@echo "  Source: source install/setup.bash"

run-seq: ## Run the sequential (Python/ROS2) pipeline
	@echo "  Starting sequential pipeline..."
	ros2 launch sequential sift_matching.launch.py \
		large_image:=ankara.jpg query_image:=test.jpg

run-par: ## Run the parallel (C++/CUDA) pipeline
	@echo "  Starting parallel (CUDA) pipeline..."
	ros2 run parallel sift_pipeline \
		--ros-args -p large_image:=ankara.jpg -p query_image:=test.jpg

benchmark: ## Run full benchmark (sequential + parallel)
	@echo "  Running full benchmark..."
	python3 scripts/benchmark.py --large ankara.jpg --query test.jpg

benchmark-quick: ## Show last benchmark results without re-running
	@echo "  Showing last benchmark results..."
	python3 scripts/benchmark.py --skip-run

# ── Documentation ─────────────────────────────────────────────────────────────

docs: ## Open HTML documentation
	@echo "  Opening documentation.html..."
	open documentation.html 2>/dev/null || \
		xdg-open documentation.html 2>/dev/null || \
		start documentation.html 2>/dev/null || \
		echo "  Open documentation.html in your browser"

# ── Development Setup ─────────────────────────────────────────────────────────

setup: install install-dev build-ros2 ## Full project setup (deps + build)
	@echo "  Project setup complete."
	@echo "  Next: source install/setup.bash"
	@echo "  Then:  make run-seq or make run-par"

.PRECIOUS: