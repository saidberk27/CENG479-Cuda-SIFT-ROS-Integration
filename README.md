# SIFT–FLANN Pipeline: Sequential vs Parallel (CUDA) Image Matching

**CENG479 — Parallel Computer Architecture and Programming — Final Project**

| Student | Number |
|---------|--------|
| **Ceren Akmeşe** | 22118080701 |
| **Said Berk** | 2111808070 |

---

## Project Overview

This project implements a **Scale-Invariant Feature Transform (SIFT)** feature extraction and **Fast Library for Approximate Nearest Neighbors (FLANN)**-based image matching pipeline in **two distinct implementations**:

1. **Sequential (Python/ROS2)** — A two-node ROS2 pipeline using CPU-based SIFT and FLANN matching, communicating via TCP sockets over ROS2 topics.
2. **Parallel (C++/CUDA)** — A fused, single-process pipeline with GPU-accelerated brute-force kNN matching, zero-copy VRAM data transport, and parallel SIFT extraction on two CPU threads.

### Key Technical Features

| Feature | Sequential (Python) | Parallel (C++/CUDA) |
|---------|-------------------|-------------------|
| **Language** | Python 3 | C++17 + CUDA 12.0 |
| **Processes** | 2 (publisher + subscriber) | 1 (fused producer + consumer) |
| **Threading** | Single thread (serial) | 2 producer threads + 1 consumer |
| **Data Transport** | ROS2 topic / DDS (TCP socket) | VRAM bus (device-pointer hand-off) |
| **Data Copies** | Serialize -> TCP -> Deserialize (5x) | 1x H2D upload, stays in VRAM |
| **kNN Algorithm** | FLANN KD-Tree (CPU, approximate) | Brute-force GPU (exact, fused ratio test) |
| **GPU** | Not used | GTX 1650 Max-Q (sm_75, 3.9 GB) |
| **Visualization** | matplotlib (2761 ms) | OpenCV imwrite (801 ms) |

---

## Architecture

### Sequential Architecture (Python/ROS2)

```
large.jpg -> SIFT (CPU) -> Serialize -> ROS2 Topic/TCP -> Deserialize -> FLANN kNN (CPU) -> RANSAC
query.jpg  -> SIFT (CPU) -> Serialize -> ROS2 Topic/TCP -------------------------------------->
```

Two separate ROS2 nodes (`sift_publisher` and `flann_subscriber`) communicate over ROS2 topics. The heavy descriptor payload (~62 MB for large images) is serialized, transmitted over DDS/TCP, and deserialized — resulting in 5x data copying overhead.

### Parallel Architecture (C++/CUDA)

```
+--------------------------- SINGLE PROCESS ---------------------------+
|  large.jpg -> SIFT (Thread 1) -> VRAM Upload -> Pub                  |
|                                                                       |
|  query.jpg  -> SIFT (Thread 2) -> VRAM Upload -> Pub                 |
|                                                                       |
|  GpuFeatureBus -> GPU kNN + Ratio (fused) -> RANSAC                   |
+-----------------------------------------------------------------------+
```

A single fused node eliminates all socket/ROS-topic overhead. The GpuFeatureBus provides an in-process, VRAM-resident publish/subscribe channel where "publishing" is an atomic pointer hand-off. Descriptors never leave VRAM once uploaded.

---

## Performance Results

| Metric | Sequential | Parallel | Speedup |
|--------|-----------|----------|---------|
| **Total pipeline wall time** | 10,754 ms | 6,191 ms | **1.74x** |
| **kNN matching** | 1,692 ms | 1,287 ms | **1.31x** |
| **Transport (data transfer)** | 1,018 ms | 0.008 ms | **~127,000x** |
| **Visualization** | 2,761 ms | 801 ms | **3.45x** |
| **Homography RANSAC** | 0.44 ms | 0.33 ms | **1.34x** |

### Stage-by-Stage Comparison

| Stage | Sequential | Parallel |
|-------|-----------|----------|
| Image Loading | 61.4 ms | — (in SIFT stage) |
| SIFT Large + Query | 1,776 ms | — (in parallel SIFT stage) |
| SIFT + Upload + Publish | — | 2,461 ms (2 threads) |
| VRAM Bus Hand-off | — | 0.008 ms |
| Data Reconstruction | 15.2 ms | — (eliminated) |
| kNN Matching | 1,692 ms | 1,287 ms |
| Lowe's Ratio Test | 2.1 ms | Fused in GPU kernel |
| Homography RANSAC | 0.44 ms | 0.33 ms |
| Visualization | 2,761 ms | 801 ms |

### Key Performance Observations

1. **Massive transport speedup (~127,000x):** The VRAM bus hand-off (0.008 ms) vs TCP socket serialization/deserialization (1,018 ms) is the single biggest win.
2. **GPU kNN matching (1.31x):** The tiled brute-force CUDA kernel outperforms CPU FLANN KD-Tree despite being an exact (not approximate) algorithm.
3. **Visualization improvement (3.45x):** OpenCV `imwrite` vs matplotlib saves significant time.
4. **SIFT on CPU (bottleneck):** The largest remaining bottleneck is SIFT feature extraction on CPU. Porting to GPU (via SIFTGPU or CV-CUDA) would yield an estimated **~2.4x additional speedup**.

---

## Project Structure

```
├── README.md                          <- This file
├── documentation.html                 <- HTML documentation
├── benchmark.py                       <- Benchmark runner
├── ankara.jpg / test.jpg              <- Test images
│
├── src/
│   ├── parallel/                      <- C++/CUDA parallel implementation
│   │   ├── include/parallel/
│   │   │   ├── cuda_kernels.hpp       <- CUDA kernel wrappers
│   │   │   ├── gpu_feature_bus.hpp    <- VRAM pub/sub bus
│   │   │   └── perf_tracker.hpp       <- C++ performance tracker
│   │   ├── src/
│   │   │   ├── sift_pipeline.cpp      <- Fused pipeline node
│   │   │   └── cuda_kernels.cu        <- CUDA kernel implementations
│   │   ├── launch/sift_matching.launch.py
│   │   ├── CMakeLists.txt
│   │   └── package.xml
│   │
│   ├── sequential/                    <- Python sequential implementation
│   │   ├── sequential/
│   │   │   ├── sift_publisher.py      <- SIFT publisher node
│   │   │   ├── flann_subscriber.py    <- FLANN subscriber node
│   │   │   └── perf_utils.py          <- Python performance tracker
│   │   ├── launch/sift_matching.launch.py
│   │   ├── setup.py / setup.cfg / package.xml
│   │   └── WALKTHROUGH.md
│   │
│   └── sift_interfaces/               <- ROS2 message definitions
│       └── msg/ImageFeatures.msg      <- SIFT feature message
```

---

## CUDA Kernel Design

The GPU kNN matching kernel (`kernel_knn_match_ratio`) uses a **tiled brute-force** approach:

- **1 warp per query** descriptor (32 threads split the 128-dim vector into 4 float4 each)
- **QUERIES_PER_BLOCK = 8** warps per block
- **Train tiles of 64 rows** loaded cooperatively into shared memory
- **Warp shuffle reduction** (`__shfl_down_sync`) for distance computation
- **Fused Lowe's ratio test** (squared-distance form, avoids sqrt)

```
Global memory traffic: 7.7K x 123K x 128 x 4 bytes = 485 GB (untiled)
                     -> (7.7K/8) x 123K x 128 x 4 bytes = 60.6 GB (tiled, ~8x reduction)
```

### Key Kernel Parameters

| Parameter | Value | Description |
|-----------|-------|-------------|
| Block size | (32, 8) | 1 warp x 8 warps = 256 threads |
| Grid size | ceil(Nq / 8) | 7.7K queries -> 962 blocks |
| Shared memory | 36.8 KB | float4 x 72 x 32 = 9,216 bytes |
| Distance | L2 squared (no sqrt) | 32-lane warp reduction |

---

## Build & Run

### Prerequisites

- ROS2 Humble
- CUDA Toolkit 12.0+
- C++17 compiler
- Python 3.10+
- OpenCV (with SIFT support)
- NVIDIA GPU (GTX 1650+ recommended)

### 1. Build with colcon

```bash
cd ~/parallel_ws
colcon build --packages-up-to parallel sequential sift_interfaces
source install/setup.bash
```

### 2. Run Sequential Pipeline

```bash
ros2 launch sequential sift_matching.launch.py \
    large_image:=ankara.jpg query_image:=test.jpg
```

### 3. Run Parallel (CUDA) Pipeline

```bash
ros2 run parallel sift_pipeline \
    --ros-args -p large_image:=ankara.jpg -p query_image:=test.jpg
```

### 4. Run Benchmark

```bash
python3 benchmark.py --large ankara.jpg --query test.jpg
```

### Output Files

- **Sequential:** `src/sequential/output/` — performance reports, pipeline images, matching result
- **Parallel:** `src/parallel/output/` — performance reports, keypoint visualizations, detected region
- **Benchmark:** `benchmark_results.json` — combined comparison data

---

## Benchmark System

The `benchmark.py` script runs both pipelines sequentially, reads their `pipeline_performance.json` outputs, and generates a comprehensive speedup comparison table. Usage:

```bash
# Run both pipelines and compare
python3 benchmark.py --large ankara.jpg --query test.jpg

# Skip running, just compare existing results
python3 benchmark.py --skip-run
```

---

## Team

| Name | Student ID | Role |
|------|-----------|------|
| **Ceren Akmeşe** | 22118080701 | CUDA Kernel Implementation, Performance Analysis |
| **Said Berk** | 2111808070 | ROS2 Pipeline Design, Sequential Implementation, Documentation |

---

## License

This project is developed for educational purposes as part of the CENG479 — Parallel Computer Architecture and Programming course at Gazi University.

---

*This README was generated from the project's documentation source `sequential_baseline_explanation.md` and benchmark data.*