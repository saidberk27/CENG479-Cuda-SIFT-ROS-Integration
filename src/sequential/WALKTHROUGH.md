# SIFT-FLANN ROS2 Matching System — Walkthrough

## Table of Contents
1. [System Overview](#system-overview)
2. [Architecture](#architecture)
3. [File Structure](#file-structure)
4. [How Each Node Works](#how-each-node-works)
5. [New: Performance Timer Additions](#new-performance-timer-additions)
6. [How to Build](#how-to-build)
7. [How to Run](#how-to-run)
8. [Example Output](#example-output)
9. [Interpreting Performance Results](#interpreting-performance-results)
10. [Customizing Parameters](#customizing-parameters)

---

## System Overview

This is a **ROS2-based image matching system** that:

1. Loads two images (a **large/train** image and a **query** image)
2. Extracts **SIFT features** (keypoints + 128-dimensional descriptors) from both
3. Publishes those features as ROS2 messages
4. A subscriber receives both feature sets and performs **FLANN-based matching**
5. Applies **Lowe's ratio test** to filter good matches
6. Computes a **homography** via RANSAC to locate the query region within the large image
7. Generates a **visualization** saved as `matching_result.png`

The pipeline is split into **two ROS2 nodes** that communicate asynchronously over topics, demonstrating a producer-consumer pattern with custom message types.

---

## Architecture

```
┌──────────────────┐          /sift/large_features      ┌──────────────────┐
│                  ├──────────────────────────────────►  │                  │
│  sift_publisher  │                                     │ flann_subscriber │
│  (Node)          ├──────────────────────────────────►  │  (Node)          │
│                  │          /sift/query_features       │                  │
└──────────────────┘                                     └──────────────────┘
                                                                    │
                                                                    ▼
                                                            matching_result.png
                                                                   + STDOUT logs
                                                                   + Performance report
```

- **`sift_publisher`** publishes once, then shuts down after a short delay.
- **`flann_subscriber`** waits for both messages, processes them, reports performance, saves the result, and shuts down automatically.

---

## File Structure

```
paralel/
├── src/
│   ├── sift_interfaces/              # Custom ROS2 message definitions
│   │   ├── msg/ImageFeatures.msg     # Message: keypoints + descriptors + image metadata
│   │   ├── CMakeLists.txt
│   │   └── package.xml
│   │
│   └── sequential/                   # ROS2 package with two nodes (renamed from sift_ros2)
│       ├── sequential/
│       │   ├── sift_publisher.py     # Node: loads images, computes SIFT, publishes
│       │   ├── flann_subscriber.py   # Node: receives features, FLANN match, homography, viz
│       │   └── perf_utils.py         # Shared performance measurement utilities
│       ├── launch/
│       │   └── sift_matching.launch.py  # Launch file to start both nodes together
│       ├── setup.py
│       ├── setup.cfg
│       └── package.xml
│
├── ankara.jpg                        # Default large/train image
├── test.jpg                          # Default query image
├── matching_result.png               # Output visualization (generated at runtime)
└── WALKTHROUGH.md                    # This file
```

### Custom Message: `ImageFeatures.msg`

```msg
string image_name
int32 height
int32 width
int32 num_keypoints
float32[] keypoints_x
float32[] keypoints_y
float32[] keypoints_size
float32[] keypoints_angle
float32[] descriptors
```

This message is defined in `src/sift_interfaces/msg/ImageFeatures.msg`.

---

## How Each Node Works

### 1. `sift_publisher.py`

**Lifecycle:**

1. **Initialization** — Declares two ROS2 parameters: `large_image` (default: `ankara.jpg`) and `query_image` (default: `test.jpg`).
2. **Image Loading** — Reads both images from disk using OpenCV. Measures load time.
3. **SIFT Extraction** — Converts to grayscale, runs `cv2.SIFT_create().detectAndCompute()` on both images. Measures SIFT computation time.
4. **Message Building** — Packs keypoint coordinates, sizes, angles, and flattened 128-d descriptors into two `ImageFeatures` messages.
5. **Publishing** — After a 500 ms delay (to let the subscriber connect), publishes both messages to topics `/sift/large_features` and `/sift/query_features`.
6. **Shutdown** — After another 2 seconds (to ensure delivery), shuts down the node.

**Published topics:**
- `/sift/large_features` — features from the large/train image
- `/sift/query_features` — features from the query image

### 2. `flann_subscriber.py`

**Lifecycle:**

1. **Initialization** — Creates two subscriptions listening to `/sift/large_features` and `/sift/query_features`. Starts a 100 ms periodic timer that checks if both messages have arrived.
   - Additionally, records `self._node_start_time` for end-to-end performance measurement.
2. **Message Reception** — Each callback logs receipt time (ms from node start), stores the message, and records the timestamp.
3. **Matching Trigger** — Once both messages are received, the timer callback `_try_match()` runs.

**`_try_match()` pipeline (each stage is timed):**

| Stage | Description |
|---|---|
| **1. Data Reconstruction** | Converts the serialized message arrays back into `cv2.KeyPoint` objects and reshapes descriptor arrays to `(N, 128)` |
| **2. FLANN kNN Matching** | Creates a `cv2.FlannBasedMatcher` (KD-Tree, 5 trees, 50 checks) and runs `knnMatch(query, train, k=2)` |
| **3. Lowe's Ratio Test** | Filters raw matches: keeps a match only if `distance_1st < 0.7 × distance_2nd` |
| **4. Point Extraction** | Extracts matched point coordinates into `src_pts` (query) and `dst_pts` (large) |
| **5. Homography (RANSAC)** | Runs `cv2.findHomography(src_pts, dst_pts, cv2.RANSAC, 5.0)` to find the transformation matrix |
| **6. Corner Computation** | Projects the query image corners onto the large image using the homography matrix |
| **7. Visualization** | Loads original images, draws the detected region on the large image, creates a 4-panel figure saved as `matching_result.png` |
| **8. Performance Report** | Logs a formatted table of all timing measurements |

---

## New: Performance Timer Additions

I added **fine-grained performance instrumentation** to `flann_subscriber.py`. The following timestamps and durations are measured:

### Measurement Points

| Measurement | Type | What it measures |
|---|---|---|
| `_node_start_time` | absolute | Time when the node's `__init__` begins |
| `_perf['recv_large']` | wall (ms) | Node init → receipt of large features message |
| `_perf['recv_query']` | wall (ms) | Node init → receipt of query features message |
| `_perf['reconstruction']` | duration (ms) | Time to reconstruct keypoints + descriptors from messages |
| `_perf['flann_matching']` | duration (ms) | Time for `knnMatch()` to run |
| `_perf['lowes_ratio_test']` | duration (ms) | Time to apply Lowe's ratio test |
| `_perf['point_extraction']` | duration (ms) | Time to extract `src_pts` / `dst_pts` arrays |
| `_perf['homography_ransac']` | duration (ms) | Time for `findHomography()` with RANSAC |
| `_perf['total_e2e']` | wall (ms) | Full end-to-end time from node start to completion |

### Output Format

At the end of processing, the node prints a formatted table:

```
----------------------------------------------------------------
  PERFORMANCE SUMMARY
----------------------------------------------------------------
  Stage                           Time (ms)   % of total
----------------------------------------------------------------
  Node init → recv large              xxx.x       xx.x%
  Node init → recv query              xxx.x       xx.x%
  Data reconstruction                 xxx.x       xx.x%
  FLANN kNN matching                  xxx.x       xx.x%
  Lowe's ratio test                   xxx.x       xx.x%
  Point extraction                    xxx.x       xx.x%
  Homography (RANSAC)                 xxx.x       xx.x%
----------------------------------------------------------------
  Total processing                    xxx.x       xx.x%
  Total E2E (node start)              xxx.x      100.0%
----------------------------------------------------------------
```

- **"Total processing"** = sum of all processing stages (reconstruction + matching + ratio test + point extraction + homography). This excludes the idle waiting time for messages to arrive.
- **"Total E2E"** = the complete wall-clock time from when the node started to when matching finished.

---

## How to Build

```bash
# Source ROS2 (adjust distro name if needed, e.g. humble, iron, rolling)
source /opt/ros/humble/setup.bash

# Navigate to workspace
cd ~/Desktop/paralel

# Build everything
colcon build
```

Or to build only the `sequential` package and its dependency on `sift_interfaces`:

```bash
colcon build --packages-up-to sequential
```

After building, source the install space:

```bash
source install/setup.bash
```

---

## How to Run

### Option 1: Using the launch file (recommended)

```bash
source install/setup.bash
ros2 launch sequential sift_matching.launch.py
```

This starts both nodes with default parameters (`large_image=ankara.jpg`, `query_image=test.jpg`).

### Option 2: Run nodes individually (in separate terminals)

**Terminal 1 — Publisher:**
```bash
source install/setup.bash
ros2 run sequential sift_publisher --ros-args -p large_image:=ankara.jpg -p query_image:=test.jpg
```

**Terminal 2 — Subscriber (with performance output):**
```bash
source install/setup.bash
ros2 run sequential flann_subscriber
```

### Using custom images

```bash
# Via launch file override
ros2 launch sequential sift_matching.launch.py large_image:=/path/to/train.jpg query_image:=/path/to/query.jpg

# Via individual nodes
ros2 run sequential sift_publisher --ros-args \
    -p large_image:=/path/to/train.jpg \
    -p query_image:=/path/to/query.jpg
```

### Monitoring topics (optional)

In a third terminal, you can inspect the published messages:

```bash
# List active topics
ros2 topic list

# Echo a feature message
ros2 topic echo /sift/large_features

# Get topic info
ros2 topic info /sift/large_features
```

---

## Example Output

When you run the system, you should see output similar to this:

```
[flann_subscriber-2] [INFO] [1718000000.123456789] [flann_subscriber]: FLANN subscriber waiting for SIFT features...
[sift_publisher-1]  [INFO] [1718000000.223456789] [sift_publisher]: Images loaded in 12.3 ms
[sift_publisher-1]  [INFO] [1718000000.523456789] [sift_publisher]: SIFT computed in 345.1 ms  (8500 keypoints)
[sift_publisher-1]  [INFO] [1718000000.723456789] [sift_publisher]: Published large features  (6200 keypoints)
[sift_publisher-1]  [INFO] [1718000000.723456789] [sift_publisher]: Published query features  (2300 keypoints)
[flann_subscriber-2] [INFO] [1718000000.823456789] [flann_subscriber]: Received large features: 6200 keypoints  (+700.3 ms from node start)
[flann_subscriber-2] [INFO] [1718000000.823456789] [flann_subscriber]: Received query features: 2300 keypoints  (+700.3 ms from node start)
[flann_subscriber-2] [INFO] [1718000000.823456789] [flann_subscriber]: Both feature sets received. Starting FLANN matching...
[flann_subscriber-2] [INFO] [1718000000.823456789] [flann_subscriber]: Reconstructed data in 15.2 ms
[flann_subscriber-2] [INFO] [1718000000.823456789] [flann_subscriber]: FLANN matching: 125.4 ms  (2300 raw matches)
[flann_subscriber-2] [INFO] [1718000000.823456789] [flann_subscriber]: Lowe's ratio test: 2.1 ms  (512 good matches)
[flann_subscriber-2] [INFO] [1718000000.823456789] [flann_subscriber]: Homography (RANSAC): 1.8 ms

----------------------------------------------------------------
  PERFORMANCE SUMMARY
----------------------------------------------------------------
  Stage                           Time (ms)   % of total
----------------------------------------------------------------
  Node init → recv large             700.3        83.1%
  Node init → recv query             700.3        83.1%
  Data reconstruction                 15.2         1.8%
  FLANN kNN matching                 125.4        14.9%
  Lowe's ratio test                    2.1         0.2%
  Point extraction                     0.3         0.0%
  Homography (RANSAC)                  1.8         0.2%
----------------------------------------------------------------
  Total processing                   144.8        17.2%
  Total E2E (node start)             842.1       100.0%
----------------------------------------------------------------

[flann_subscriber-2] [INFO] [1718000000.923456789] [flann_subscriber]: Matching region corners on large image:
[flann_subscriber-2] [INFO] [1718000000.923456789] [flann_subscriber]:   Corner 1: (123.4, 567.8)
[flann_subscriber-2] [INFO] [1718000000.923456789] [flann_subscriber]:   Corner 2: (234.5, 678.9)
[flann_subscriber-2] [INFO] [1718000000.923456789] [flann_subscriber]:   Corner 3: (345.6, 789.0)
[flann_subscriber-2] [INFO] [1718000000.923456789] [flann_subscriber]:   Corner 4: (456.7, 890.1)
[flann_subscriber-2] [INFO] [1718000000.923456789] [flann_subscriber]: Result saved to matching_result.png
[flann_subscriber-2] [INFO] [1718000000.923456789] [flann_subscriber]: FLANN matching complete. Shutting down.
```

---

## Interpreting Performance Results

## Understanding "Wall ms" (Wall Clock Time)

The `Wall (ms)` column in the performance report measures **wall clock time** — the real-world elapsed time as measured by a clock on the wall. This is the time you would measure with a stopwatch.

Key differences:
- **Wall time** = actual elapsed real-world time (includes time spent waiting for I/O, context switches, other processes, etc.)
- **CPU time** = time the CPU actually spent executing instructions of this process

When CPU time is close to wall time, the process was running continuously without waiting. When CPU time is much less than wall time, the process spent significant time waiting (e.g., for I/O, network, or other resources).

The performance table breaks down where time is spent:

1. **`Node init → recv large/query`** — The bulk of time (typically 80-90% of E2E). This is **waiting time** while the publisher loads images, computes SIFT, and transmits the messages. This idle time is dominated by SIFT computation on the publisher side.

2. **`Data reconstruction`** — Deserialization overhead. Small for modest keypoint counts (< 50 ms for < 10k keypoints).

3. **`FLANN kNN matching`** — Usually the **largest processing stage** (10-15% of E2E). This grows with `O(N_query × kd_tree_search_time)`. For 2-6k descriptors, expect ~50-200 ms. You can tune this by changing `trees` (index_params) and `checks` (search_params) in the code:
   - More `trees` → faster matching, slower index build
   - More `checks` → more accurate matches, slower search

4. **`Lowe's ratio test`** — Very fast (< 5 ms). Scales linearly with the number of raw matches.

5. **`Homography (RANSAC)`** — Fast for moderate match counts (< 10 ms for < 1000 good matches). RANSAC's runtime depends on the number of iterations (adaptive) and the number of input points.

6. **`Total processing`** — The sum of all non-idle stages. This reflects the true computation cost (network-free).

---

## Customizing Parameters

### SIFT Parameters (in `sift_publisher.py`)

```python
# To change SIFT parameters, modify the SIFT_create() call:
sift = cv2.SIFT_create(
    nfeatures=0,       # default: 0 (no limit)
    nOctaveLayers=3,   # default: 3
    contrastThreshold=0.04,  # default: 0.04
    edgeThreshold=10,        # default: 10
    sigma=1.6          # default: 1.6
)
```

### FLANN Parameters (in `flann_subscriber.py`)

```python
FLANN_INDEX_KDTREE = 1
index_params = dict(algorithm=FLANN_INDEX_KDTREE, trees=5)     # trees: more = faster search
search_params = dict(checks=50)                                 # checks: more = more accurate
```

### Lowe's Ratio Threshold (in `flann_subscriber.py`)

```python
if m.distance < 0.7 * n.distance:   # Lower = stricter filtering
```

- **0.7** (default) — good for most cases
- **0.6** — stricter, fewer but higher quality matches
- **0.8** — more matches, but more false positives