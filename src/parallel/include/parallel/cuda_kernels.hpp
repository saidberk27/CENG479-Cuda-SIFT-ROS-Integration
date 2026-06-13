#pragma once
/**
 * cuda_kernels.hpp
 * ─────────────────────────────────────────────────────────────────────────────
 * Host-callable wrappers around the CUDA kernels in cuda_kernels.cu.
 * Include this header from pure C++ translation units (.cpp).
 *
 * Optimised CUDA-aware path:
 *   • upload_features_to_gpu()  – copy a SIFT feature set into VRAM once.
 *   • cuda_knn_match_ratio()    – GPU brute-force kNN (L2) matching + Lowe's
 *                                 ratio test, run ENTIRELY on the descriptors
 *                                 that already live in VRAM.  No descriptor
 *                                 ever returns to the CPU; only the tiny match
 *                                 index/result arrays come back.
 *
 * The legacy host-side helpers (cuda_bgr_to_gray, cuda_lowe_ratio_test) are
 * retained for compatibility but are no longer on the hot path.
 */

#include <memory>
#include <opencv2/core.hpp>
#include <opencv2/features2d.hpp>
#include <string>
#include <vector>

namespace parallel
{

    struct GpuFeatures; // fwd-decl (defined in gpu_feature_bus.hpp)

    /**
     * Upload a host SIFT feature set (descriptors + keypoint fields) into VRAM.
     *
     * The returned GpuFeatures owns device buffers; they are freed automatically
     * when the shared_ptr is released (custom deleter wired up inside).
     *
     * This is the ONLY host→device transfer of the heavy data and it is done with
     * pinned memory + an async stream so it overlaps with compute where possible.
     *
     * @param descriptors  CV_32F, num_keypoints x 128 (host memory)
     * @param keypoints    SIFT keypoints (host memory)
     * @param image_name   metadata label
     * @param height,width image dimensions (for downstream homography)
     */
    std::shared_ptr<GpuFeatures> upload_features_to_gpu(
        const cv::Mat &descriptors,
        const std::vector<cv::KeyPoint> &keypoints,
        const std::string &image_name,
        int height, int width);

    /**
     * Result of a GPU kNN + ratio-test match.  Only these small arrays cross back
     * to the host — the descriptors stayed in VRAM throughout.
     */
    struct GpuMatchResult
    {
        // For each query keypoint that PASSED the ratio test:
        std::vector<int> query_idx; // index into query feature set
        std::vector<int> train_idx; // matched index into train/large feature set
        int total_query = 0;        // number of query descriptors examined
        int good_matches = 0;       // number that passed the ratio test
    };

    /**
     * GPU brute-force kNN (k=2, L2) matching + Lowe's ratio test, computed directly
     * on the VRAM-resident descriptor buffers of the two feature sets.
     *
     * For each query descriptor q the kernel finds the two nearest train
     * descriptors (best, second) and keeps q iff best < ratio^2 * second
     * (squared-distance form of Lowe's test, avoids sqrt).
     *
     * @param query   query feature set (device buffers)
     * @param train   train/large feature set (device buffers)
     * @param ratio   Lowe's ratio (default 0.7)
     */
    GpuMatchResult cuda_knn_match_ratio(
        const GpuFeatures &query,
        const GpuFeatures &train,
        float ratio = 0.7f);

    // ── Legacy helpers (kept for compatibility, not on the hot path) ────────────
    cv::Mat cuda_bgr_to_gray(const cv::Mat &bgr_img);

    std::vector<bool> cuda_lowe_ratio_test(
        const std::vector<std::vector<cv::DMatch>> &knn_matches,
        float ratio = 0.7f);

    /** Return a human-readable string describing the active CUDA device. */
    std::string cuda_device_info();

} // namespace parallel