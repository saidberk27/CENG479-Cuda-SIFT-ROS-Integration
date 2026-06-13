#pragma once
/**
 * gpu_feature_bus.hpp
 * ─────────────────────────────────────────────────────────────────────────────
 * In-process, VRAM-resident publish/subscribe bus.
 *
 * Design goals (per optimisation request):
 *   • NO sockets, NO ROS-topic serialization for the heavy feature payload.
 *   • Publisher and subscriber share the SAME memory space (single process).
 *   • The shared data lives in VRAM — only device pointers and small scalar
 *     counters are exchanged across the bus.  Nothing big ever touches the CPU.
 *   • CUDA-aware: the consumer reads the producer's device buffers directly.
 *
 * A "publish" therefore costs an atomic pointer hand-off (a few nanoseconds),
 * not a memcpy / serialization / socket send.  This is the maximum-parallelism,
 * zero-copy path: the GPU owns the data start-to-finish.
 */

#include <atomic>
#include <condition_variable>
#include <cstdint>
#include <memory>
#include <mutex>
#include <string>

namespace parallel
{

    /**
     * GpuFeatures — a handle to a SIFT feature set that lives entirely in VRAM.
     *
     * All pointers below are *device* pointers (cudaMalloc'd).  The struct itself
     * is tiny and is what travels across the bus; the descriptors (N x 128 floats)
     * never leave the GPU.
     */
    struct GpuFeatures
    {
        // Device buffers (VRAM-resident)
        float *d_descriptors = nullptr; // N x 128, row-major, device memory
        float *d_kp_x = nullptr;        // N, device memory
        float *d_kp_y = nullptr;        // N, device memory
        float *d_kp_size = nullptr;     // N, device memory
        float *d_kp_angle = nullptr;    // N, device memory

        int num_keypoints = 0;
        int descriptor_dim = 128;
        int height = 0;
        int width = 0;

        std::string image_name; // tiny metadata only (CPU-side label)

        // Producer-set timestamp (seconds since epoch) for latency accounting only.
        double publish_timestamp = 0.0;

        bool valid() const { return d_descriptors != nullptr && num_keypoints > 0; }
    };

    /**
     * GpuFeatureBus — single-process, two-slot VRAM pub/sub channel.
     *
     * The publisher calls publish() with a GPU-resident GpuFeatures handle for each
     * of the two images (query + large/train).  The subscriber blocks in
     * wait_for_pair() until both handles are available, then consumes them in-place.
     *
     * Because both ends are in the same process and address space, the "transport"
     * is a shared_ptr hand-off guarded by a mutex/condvar — no copy of the payload,
     * which stays in VRAM the entire time.
     */
    class GpuFeatureBus
    {
    public:
        enum class Slot
        {
            kQuery = 0,
            kLarge = 1
        };

        void publish(Slot slot, std::shared_ptr<GpuFeatures> features)
        {
            {
                std::lock_guard<std::mutex> lk(mtx_);
                slots_[static_cast<int>(slot)] = std::move(features);
            }
            cv_.notify_all();
        }

        /** Block until both slots are populated. Returns {query, large}. */
        std::pair<std::shared_ptr<GpuFeatures>, std::shared_ptr<GpuFeatures>>
        wait_for_pair()
        {
            std::unique_lock<std::mutex> lk(mtx_);
            cv_.wait(lk, [this]
                     { return slots_[0] != nullptr && slots_[1] != nullptr; });
            return {slots_[0], slots_[1]};
        }

    private:
        std::mutex mtx_;
        std::condition_variable cv_;
        std::shared_ptr<GpuFeatures> slots_[2];
    };

} // namespace parallel