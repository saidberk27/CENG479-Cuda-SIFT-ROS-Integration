/**
 * cuda_kernels.cu
 * ─────────────────────────────────────────────────────────────────────────────
 * CUDA kernels for the paralel ROS2 pipeline.
 *
 * Optimised CUDA-aware data path:
 *   • upload_features_to_gpu()  – uploads descriptors+keypoints into VRAM ONCE
 *                                 (pinned host staging + async stream).
 *   • cuda_knn_match_ratio()    – brute-force kNN (k=2, L2) matching fused with
 *                                 Lowe's ratio test, executed directly on the
 *                                 VRAM-resident descriptor buffers.  The heavy
 *                                 data never returns to the CPU; only the tiny
 *                                 match-index arrays do.
 *
 * Legacy kernels (kernel_bgr_to_gray, kernel_lowe_ratio) are retained but are
 * no longer used on the hot path.
 */

#include "paralel/cuda_kernels.hpp"
#include "paralel/gpu_feature_bus.hpp"

#include <cuda_runtime.h>
#include <device_launch_parameters.h>
#include <float.h>
#include <stdexcept>
#include <string>

// ─────────────────────────────────────────────────────────────────────────────
// Helpers
// ─────────────────────────────────────────────────────────────────────────────
#define CUDA_CHECK(call)                                                        \
    do {                                                                        \
        cudaError_t _e = (call);                                                \
        if (_e != cudaSuccess) {                                                \
            throw std::runtime_error(                                           \
                std::string("CUDA error at " __FILE__ ":") +                   \
                std::to_string(__LINE__) + " – " +                             \
                cudaGetErrorString(_e));                                        \
        }                                                                       \
    } while (0)

static constexpr int DESC_DIM = 128;

// ─────────────────────────────────────────────────────────────────────────────
// Kernel: brute-force kNN (k=2, squared-L2) + fused Lowe ratio test
//
// One thread block per query descriptor.  The block cooperatively loads the
// query descriptor (128 floats) into shared memory, then strides over all
// train descriptors computing squared-L2 distance; each thread keeps its own
// best/second, then a block reduction finds the global best/second.
//
// Lowe's ratio test in squared form (avoids sqrt):
//   keep  iff  best < (ratio^2) * second
//
// Outputs (device):
//   d_train_idx[q]  – index of nearest train descriptor (or -1)
//   d_pass[q]       – 1 if the pair passes the ratio test, else 0
// ─────────────────────────────────────────────────────────────────────────────
// Tiled kNN: one WARP per query, QUERIES_PER_BLOCK warps per block.
//
// Train descriptors are streamed through shared memory in tiles of TILE_T rows.
// All warps in the block cooperatively load each train tile ONCE into shared
// memory, then every query (warp) reuses it.  This turns the O(Nq*Nt) global
// reads of the train matrix into O((Nq/QUERIES_PER_BLOCK)*Nt) — a big cut in
// memory traffic, which is the bottleneck for this asymmetric problem.
//
// Within a warp, the 32 lanes split the 128-dim distance (4 float4 each) and
// reduce with __shfl_down_sync.  Each warp keeps its query's best/second.
static constexpr int WARP             = 32;
static constexpr int QUERIES_PER_BLOCK = 8;   // = warps per block
static constexpr int TILE_T            = 64;  // train rows cached per tile

__global__ void kernel_knn_match_ratio(
    const float * __restrict__ d_query,   // Nq x 128
    const float * __restrict__ d_train,   // Nt x 128
    int Nq, int Nt,
    float ratio_sq,
    int * __restrict__ d_train_idx,
    unsigned char * __restrict__ d_pass)
{
    const int lane    = threadIdx.x;                 // 0..31
    const int warpId  = threadIdx.y;                 // 0..QUERIES_PER_BLOCK-1
    const int q = blockIdx.x * QUERIES_PER_BLOCK + warpId;

    // Shared tile of train descriptors (TILE_T x 128), plus this block's query
    // descriptors (QUERIES_PER_BLOCK x 128).
    __shared__ float4 s_train[TILE_T][DESC_DIM / 4];
    __shared__ float4 s_q[QUERIES_PER_BLOCK][DESC_DIM / 4];

    const int nthreads = WARP * QUERIES_PER_BLOCK;
    const int tid      = warpId * WARP + lane;

    // Load this warp's query descriptor into shared (each lane loads 1 float4).
    if (q < Nq) {
        const float4 * q4 = reinterpret_cast<const float4 *>(d_query + q * DESC_DIM);
        s_q[warpId][lane] = __ldg(&q4[lane]);   // DESC_DIM/4 == 32 == WARP
    }

    float best = FLT_MAX, second = FLT_MAX;
    int   best_idx = -1;

    for (int t0 = 0; t0 < Nt; t0 += TILE_T) {
        const int tile = min(TILE_T, Nt - t0);

        // Cooperatively load the train tile into shared memory (coalesced).
        // Each thread loads multiple float4 elements.
        for (int e = tid; e < tile * (DESC_DIM / 4); e += nthreads) {
            int row = e / (DESC_DIM / 4);
            int col = e % (DESC_DIM / 4);
            const float4 * tr = reinterpret_cast<const float4 *>(
                d_train + (t0 + row) * DESC_DIM);
            s_train[row][col] = __ldg(&tr[col]);
        }
        __syncthreads();

        if (q < Nq) {
            float4 qv = s_q[warpId][lane];   // this lane's slice of the query
            for (int r = 0; r < tile; ++r) {
                float4 tv = s_train[r][lane];
                float dx = qv.x - tv.x;
                float dy = qv.y - tv.y;
                float dz = qv.z - tv.z;
                float dw = qv.w - tv.w;
                float partial = dx * dx + dy * dy + dz * dz + dw * dw;
                // Warp reduction across the 32 lanes -> full 128-dim distance.
                #pragma unroll
                for (int off = 16; off > 0; off >>= 1)
                    partial += __shfl_down_sync(0xffffffff, partial, off);
                float dist = __shfl_sync(0xffffffff, partial, 0);

                int t = t0 + r;
                if (dist < best) {
                    second = best; best = dist; best_idx = t;
                } else if (dist < second) {
                    second = dist;
                }
            }
        }
        __syncthreads();
    }

    if (q < Nq && lane == 0) {
        d_train_idx[q] = best_idx;
        d_pass[q] = (best < ratio_sq * second) ? 1u : 0u;
    }
}


// ─────────────────────────────────────────────────────────────────────────────
// Legacy Kernel: BGR → Grayscale  (kept, not on hot path)
// ─────────────────────────────────────────────────────────────────────────────
__global__ void kernel_bgr_to_gray(
    const unsigned char * __restrict__ d_bgr,
    unsigned char       * __restrict__ d_gray,
    int width, int height)
{
    int x = blockIdx.x * blockDim.x + threadIdx.x;
    int y = blockIdx.y * blockDim.y + threadIdx.y;
    if (x >= width || y >= height) return;

    int idx_bgr  = (y * width + x) * 3;
    int idx_gray =  y * width + x;

    float b = static_cast<float>(d_bgr[idx_bgr + 0]);
    float g = static_cast<float>(d_bgr[idx_bgr + 1]);
    float r = static_cast<float>(d_bgr[idx_bgr + 2]);

    float lum = 0.114f * b + 0.587f * g + 0.299f * r;
    d_gray[idx_gray] = static_cast<unsigned char>(lum + 0.5f);
}

// ─────────────────────────────────────────────────────────────────────────────
// Legacy Kernel: Lowe's ratio test (host-fed distances) — kept, not on hot path
// ─────────────────────────────────────────────────────────────────────────────
__global__ void kernel_lowe_ratio(
    const float * __restrict__ d_dist1,
    const float * __restrict__ d_dist2,
    unsigned char * __restrict__ d_mask,
    int N,
    float ratio)
{
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    if (i >= N) return;
    d_mask[i] = (d_dist1[i] < ratio * d_dist2[i]) ? 1u : 0u;
}

// ─────────────────────────────────────────────────────────────────────────────
namespace paralel {

// ── Device-buffer deleter for GpuFeatures ────────────────────────────────────
static void free_gpu_features(GpuFeatures * f)
{
    if (!f) return;
    if (f->d_descriptors) cudaFree(f->d_descriptors);
    if (f->d_kp_x)        cudaFree(f->d_kp_x);
    if (f->d_kp_y)        cudaFree(f->d_kp_y);
    if (f->d_kp_size)     cudaFree(f->d_kp_size);
    if (f->d_kp_angle)    cudaFree(f->d_kp_angle);
    delete f;
}

// ── upload_features_to_gpu ────────────────────────────────────────────────────
std::shared_ptr<GpuFeatures> upload_features_to_gpu(
    const cv::Mat & descriptors,
    const std::vector<cv::KeyPoint> & keypoints,
    const std::string & image_name,
    int height, int width)
{
    const int N = static_cast<int>(keypoints.size());

    std::shared_ptr<GpuFeatures> gf(new GpuFeatures(), free_gpu_features);
    gf->num_keypoints  = N;
    gf->descriptor_dim = DESC_DIM;
    gf->height         = height;
    gf->width          = width;
    gf->image_name     = image_name;

    if (N == 0) return gf;

    const size_t desc_bytes = static_cast<size_t>(N) * DESC_DIM * sizeof(float);
    const size_t kp_bytes   = static_cast<size_t>(N) * sizeof(float);

    // Allocate VRAM buffers (these persist across the bus hand-off).
    CUDA_CHECK(cudaMalloc(&gf->d_descriptors, desc_bytes));
    CUDA_CHECK(cudaMalloc(&gf->d_kp_x,    kp_bytes));
    CUDA_CHECK(cudaMalloc(&gf->d_kp_y,    kp_bytes));
    CUDA_CHECK(cudaMalloc(&gf->d_kp_size, kp_bytes));
    CUDA_CHECK(cudaMalloc(&gf->d_kp_angle, kp_bytes));

    // Stage keypoint SoA arrays.
    std::vector<float> kx(N), ky(N), ks(N), ka(N);
    for (int i = 0; i < N; ++i) {
        kx[i] = keypoints[i].pt.x;
        ky[i] = keypoints[i].pt.y;
        ks[i] = keypoints[i].size;
        ka[i] = keypoints[i].angle;
    }

    // Ensure contiguous CV_32F descriptors.
    cv::Mat desc = descriptors;
    if (desc.type() != CV_32F) desc.convertTo(desc, CV_32F);
    if (!desc.isContinuous())  desc = desc.clone();

    // Async copies on a dedicated stream — this is the single, one-time H2D
    // transfer of the heavy descriptor block.
    cudaStream_t stream;
    CUDA_CHECK(cudaStreamCreate(&stream));
    CUDA_CHECK(cudaMemcpyAsync(gf->d_descriptors, desc.ptr<float>(0),
                               desc_bytes, cudaMemcpyHostToDevice, stream));
    CUDA_CHECK(cudaMemcpyAsync(gf->d_kp_x, kx.data(), kp_bytes,
                               cudaMemcpyHostToDevice, stream));
    CUDA_CHECK(cudaMemcpyAsync(gf->d_kp_y, ky.data(), kp_bytes,
                               cudaMemcpyHostToDevice, stream));
    CUDA_CHECK(cudaMemcpyAsync(gf->d_kp_size, ks.data(), kp_bytes,
                               cudaMemcpyHostToDevice, stream));
    CUDA_CHECK(cudaMemcpyAsync(gf->d_kp_angle, ka.data(), kp_bytes,
                               cudaMemcpyHostToDevice, stream));
    CUDA_CHECK(cudaStreamSynchronize(stream));
    CUDA_CHECK(cudaStreamDestroy(stream));

    return gf;
}

// ── cuda_knn_match_ratio ──────────────────────────────────────────────────────
GpuMatchResult cuda_knn_match_ratio(
    const GpuFeatures & query,
    const GpuFeatures & train,
    float ratio)
{
    GpuMatchResult out;
    out.total_query = query.num_keypoints;

    const int Nq = query.num_keypoints;
    const int Nt = train.num_keypoints;
    if (Nq == 0 || Nt == 0) return out;

    int           *d_train_idx = nullptr;
    unsigned char *d_pass      = nullptr;
    CUDA_CHECK(cudaMalloc(&d_train_idx, Nq * sizeof(int)));
    CUDA_CHECK(cudaMalloc(&d_pass,      Nq * sizeof(unsigned char)));

    // One warp per query, QUERIES_PER_BLOCK warps per block.  Train descriptors
    // are streamed through static shared memory inside the kernel (no dynamic
    // shared memory needed here).
    const dim3 block(WARP, QUERIES_PER_BLOCK);
    const dim3 grid((Nq + QUERIES_PER_BLOCK - 1) / QUERIES_PER_BLOCK);

    const float ratio_sq = ratio * ratio;

    kernel_knn_match_ratio<<<grid, block>>>(
        query.d_descriptors, train.d_descriptors,
        Nq, Nt, ratio_sq, d_train_idx, d_pass);

    CUDA_CHECK(cudaGetLastError());
    CUDA_CHECK(cudaDeviceSynchronize());

    // Only the tiny result arrays come back to the host.
    std::vector<int>           h_train_idx(Nq);
    std::vector<unsigned char> h_pass(Nq);
    CUDA_CHECK(cudaMemcpy(h_train_idx.data(), d_train_idx,
                          Nq * sizeof(int), cudaMemcpyDeviceToHost));
    CUDA_CHECK(cudaMemcpy(h_pass.data(), d_pass,
                          Nq * sizeof(unsigned char), cudaMemcpyDeviceToHost));

    cudaFree(d_train_idx);
    cudaFree(d_pass);

    out.query_idx.reserve(Nq);
    out.train_idx.reserve(Nq);
    for (int q = 0; q < Nq; ++q) {
        if (h_pass[q] && h_train_idx[q] >= 0) {
            out.query_idx.push_back(q);
            out.train_idx.push_back(h_train_idx[q]);
        }
    }
    out.good_matches = static_cast<int>(out.query_idx.size());
    return out;
}

// ── cuda_bgr_to_gray (legacy) ────────────────────────────────────────────────
cv::Mat cuda_bgr_to_gray(const cv::Mat & bgr_img)
{
    if (bgr_img.empty() || bgr_img.channels() != 3) {
        throw std::invalid_argument("cuda_bgr_to_gray: expected non-empty 3-channel BGR image");
    }

    const int W = bgr_img.cols;
    const int H = bgr_img.rows;
    const size_t bgr_bytes  = static_cast<size_t>(W) * H * 3;
    const size_t gray_bytes = static_cast<size_t>(W) * H;

    cv::Mat bgr_cont = bgr_img.isContinuous() ? bgr_img : bgr_img.clone();

    unsigned char *d_bgr  = nullptr;
    unsigned char *d_gray = nullptr;
    CUDA_CHECK(cudaMalloc(&d_bgr,  bgr_bytes));
    CUDA_CHECK(cudaMalloc(&d_gray, gray_bytes));

    CUDA_CHECK(cudaMemcpy(d_bgr, bgr_cont.data, bgr_bytes, cudaMemcpyHostToDevice));

    dim3 block(32, 16);
    dim3 grid((W + block.x - 1) / block.x,
              (H + block.y - 1) / block.y);
    kernel_bgr_to_gray<<<grid, block>>>(d_bgr, d_gray, W, H);
    CUDA_CHECK(cudaGetLastError());
    CUDA_CHECK(cudaDeviceSynchronize());

    cv::Mat gray(H, W, CV_8UC1);
    CUDA_CHECK(cudaMemcpy(gray.data, d_gray, gray_bytes, cudaMemcpyDeviceToHost));

    cudaFree(d_bgr);
    cudaFree(d_gray);

    return gray;
}

// ── cuda_lowe_ratio_test (legacy) ────────────────────────────────────────────
std::vector<bool> cuda_lowe_ratio_test(
    const std::vector<std::vector<cv::DMatch>> & knn_matches,
    float ratio)
{
    const int N = static_cast<int>(knn_matches.size());
    std::vector<bool> result(N, false);
    if (N == 0) return result;

    std::vector<float> h_dist1(N), h_dist2(N);
    for (int i = 0; i < N; ++i) {
        if (knn_matches[i].size() >= 2) {
            h_dist1[i] = knn_matches[i][0].distance;
            h_dist2[i] = knn_matches[i][1].distance;
        } else {
            h_dist1[i] = 1.0f;
            h_dist2[i] = 0.0f;
        }
    }

    float         *d_dist1 = nullptr;
    float         *d_dist2 = nullptr;
    unsigned char *d_mask  = nullptr;
    CUDA_CHECK(cudaMalloc(&d_dist1, N * sizeof(float)));
    CUDA_CHECK(cudaMalloc(&d_dist2, N * sizeof(float)));
    CUDA_CHECK(cudaMalloc(&d_mask,  N * sizeof(unsigned char)));

    CUDA_CHECK(cudaMemcpy(d_dist1, h_dist1.data(), N * sizeof(float), cudaMemcpyHostToDevice));
    CUDA_CHECK(cudaMemcpy(d_dist2, h_dist2.data(), N * sizeof(float), cudaMemcpyHostToDevice));

    const int BLOCK = 256;
    const int GRID  = (N + BLOCK - 1) / BLOCK;
    kernel_lowe_ratio<<<GRID, BLOCK>>>(d_dist1, d_dist2, d_mask, N, ratio);
    CUDA_CHECK(cudaGetLastError());
    CUDA_CHECK(cudaDeviceSynchronize());

    std::vector<unsigned char> h_mask(N);
    CUDA_CHECK(cudaMemcpy(h_mask.data(), d_mask, N * sizeof(unsigned char), cudaMemcpyDeviceToHost));

    cudaFree(d_dist1);
    cudaFree(d_dist2);
    cudaFree(d_mask);

    for (int i = 0; i < N; ++i) result[i] = (h_mask[i] != 0);
    return result;
}

// ── cuda_device_info ─────────────────────────────────────────────────────────
std::string cuda_device_info()
{
    int dev = 0;
    cudaDeviceProp prop{};
    cudaError_t e = cudaGetDeviceProperties(&prop, dev);
    if (e != cudaSuccess) return "CUDA device query failed";

    char buf[512];
    snprintf(buf, sizeof(buf),
             "%s  (sm_%d%d, %.0f MB, %d SMs, CUDA %d.%d)",
             prop.name,
             prop.major, prop.minor,
             prop.totalGlobalMem / 1024.0 / 1024.0,
             prop.multiProcessorCount,
             prop.major, prop.minor);
    return buf;
}

} // namespace paralel
