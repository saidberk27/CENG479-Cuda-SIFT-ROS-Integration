cmake_minimum_required(VERSION 3.18)
project(paralel CUDA CXX)

# ── C++ / CUDA standards ─────────────────────────────────────────────────────
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CUDA_STANDARD 17)
set(CMAKE_CUDA_STANDARD_REQUIRED ON)

# ── Compiler warnings ────────────────────────────────────────────────────────
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# ── CUDA architecture (GTX 1650 = sm_75) ────────────────────────────────────
# GTX 1650 is Turing architecture = sm_75
set(CMAKE_CUDA_ARCHITECTURES 75)
# Toolkit is CUDA 12.0 from /usr/bin/nvcc
set(CMAKE_CUDA_COMPILER /usr/bin/nvcc)

# ── ROS2 / ament dependencies ────────────────────────────────────────────────
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(sift_interfaces REQUIRED)
find_package(sensor_msgs REQUIRED)

# ── CUDA ─────────────────────────────────────────────────────────────────────
find_package(CUDAToolkit REQUIRED)

# ── OpenCV (for SIFT, FLANN, visualization) ──────────────────────────────────
find_package(OpenCV REQUIRED)

# ── Output directory for performance reports ─────────────────────────────────
set(PARALEL_OUTPUT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/output")
file(MAKE_DIRECTORY ${PARALEL_OUTPUT_DIR})

# ── Include directories ──────────────────────────────────────────────────────
include_directories(
  include
  ${OpenCV_INCLUDE_DIRS}
)

# ── CUDA kernel library ──────────────────────────────────────────────────────
add_library(paralel_cuda_kernels STATIC
  src/cuda_kernels.cu
)
set_target_properties(paralel_cuda_kernels PROPERTIES
  CUDA_SEPARABLE_COMPILATION ON
  POSITION_INDEPENDENT_CODE ON
)
target_include_directories(paralel_cuda_kernels PUBLIC
  include
  ${CUDAToolkit_INCLUDE_DIRS}
  ${OpenCV_INCLUDE_DIRS}
)
target_link_libraries(paralel_cuda_kernels
  CUDA::cudart
  CUDA::cublas
  ${OpenCV_LIBS}
)

# ── sift_pipeline executable (fused producer+consumer, VRAM pub/sub) ─────────
# Single process: SIFT producer and FLANN/match consumer share the same address
# space AND the same GPU memory.  "Publish/subscribe" is a device-pointer
# hand-off through an in-process VRAM bus — no sockets, no ROS-topic payload.
add_executable(sift_pipeline
  src/sift_pipeline.cpp
)
target_include_directories(sift_pipeline PRIVATE
  include
  ${CUDAToolkit_INCLUDE_DIRS}
  ${OpenCV_INCLUDE_DIRS}
)
target_link_libraries(sift_pipeline
  paralel_cuda_kernels
  CUDA::cudart
  ${OpenCV_LIBS}
)
ament_target_dependencies(sift_pipeline
  rclcpp
  std_msgs
  sift_interfaces
  sensor_msgs
)

# ── Install targets ──────────────────────────────────────────────────────────
install(TARGETS
  sift_pipeline
  DESTINATION lib/${PROJECT_NAME}
)


install(DIRECTORY launch/
  DESTINATION share/${PROJECT_NAME}/launch
)

install(DIRECTORY include/
  DESTINATION include
)

# ── Linting ──────────────────────────────────────────────────────────────────
if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
