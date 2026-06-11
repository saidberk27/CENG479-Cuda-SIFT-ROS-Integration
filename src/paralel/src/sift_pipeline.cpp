m/**
 * sift_pipeline.cpp
 * ─────────────────────────────────────────────────────────────────────────────
 * Fused, single-process, CUDA-aware SIFT → FLANN pipeline.
 *
 * WHY A SINGLE PROCESS?
 *   The previous design ran a sift_publisher and a flann_subscriber as two
 *   separate processes that exchanged the (large) descriptor payload over a TCP
 *   socket AND over ROS2 topics.  Both transports force the data through the
 *   CPU (serialize → copy → kernel buffers → copy → deserialize).
 *
 *   This version fuses producer and consumer into ONE process so that
 *   "publish / subscribe" is a hand-off of *device pointers* through an
 *   in-process, VRAM-resident bus (GpuFeatureBus).  Consequences:
 *
 *     • NO sockets.            (removed entirely)
 *     • NO ROS-topic payload.  (the heavy data never leaves VRAM)
 *     • Publisher & subscriber share the SAME address space AND the same GPU
 *       memory — the descriptors live in VRAM start-to-finish.
 *     • CUDA-aware: the consumer's match kernel reads the producer's device
 *       buffers directly; the only thing crossing back to the CPU is the tiny
 *       match-index array and, finally, the PERFORMANCE NUMBERS.
 *
 * MAXIMUM PARALLELISM:
 *   • The two images' SIFT extractions run on two threads (half cores each).
 *   • Each feature set is uploaded to VRAM and *published* as soon as it is
 *     ready, so the consumer can begin as soon as both are present.
 *   • The consumer runs the entire match on the GPU (brute-force kNN + Lowe
 *     ratio test fused in one kernel).
 *   • Optional visualization (imwrite) is off the critical path.
 *
 * Pipeline:
 *   [Producer thread x2]  load → grayscale → SIFT → upload-to-VRAM → publish
 *   [Consumer thread]     wait_for_pair → GPU kNN+ratio → RANSAC → perf report
 */

#include <chrono>
#include <filesystem>
#include <future>
#include <memory>
#include <string>
#include <thread>
#include <vector>

// OpenCV
#include <opencv2/calib3d.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

// ROS2
#include "rclcpp/rclcpp.hpp"

// Local
#include "paralel/cuda_kernels.hpp"
#include "paralel/gpu_feature_bus.hpp"
#include "paralel/perf_tracker.hpp"

namespace fs = std::filesystem;

static double now_epoch_s()
{
    return std::chrono::duration<double>(
        std::chrono::system_clock::now().time_since_epoch()).count();
}

// ─────────────────────────────────────────────────────────────────────────────
// SiftPipeline node  —  fused producer + consumer in one process
// ─────────────────────────────────────────────────────────────────────────────
class SiftPipeline : public rclcpp::Node
{
public:
    SiftPipeline()
    : Node("sift_pipeline")
    {
        declare_parameter("large_image", "ankara.jpg");
        declare_parameter("query_image", "test.jpg");
        declare_parameter("output_dir",
            (fs::current_path() / "src" / "paralel" / "output").string());
        declare_parameter("save_visualizations", true);

        large_path_  = get_parameter("large_image").as_string();
        query_path_  = get_parameter("query_image").as_string();
        out_dir_     = get_parameter("output_dir").as_string();
        save_vis_    = get_parameter("save_visualizations").as_bool();

        fs::create_directories(out_dir_);

        perf_ = std::make_unique<paralel::PerfTracker>("sift_pipeline", out_dir_);
        perf_->add_info("large_image_path", large_path_);
        perf_->add_info("query_image_path", query_path_);
        perf_->add_info("cuda_device", paralel::cuda_device_info());
        perf_->add_info("transport", "in-process VRAM bus (no sockets, no ROS payload)");

        RCLCPP_INFO(get_logger(), "CUDA device: %s",
            paralel::cuda_device_info().c_str());
        RCLCPP_INFO(get_logger(),
            "Transport: in-process VRAM pub/sub (zero-copy, CUDA-aware)");

        // Kick off the whole pipeline shortly after construction so the node is
        // fully spun up.
        start_timer_ = create_wall_timer(
            std::chrono::milliseconds(10),
            std::bind(&SiftPipeline::run_once, this));
    }

private:
    // ── One feature set produced and published to the VRAM bus ───────────────
    void produce(const std::string & path,
                 paralel::GpuFeatureBus::Slot slot,
                 const std::string & tag,
                 int half_cores)
    {
        auto log = get_logger();

        // Load + grayscale (CPU SIMD; PCIe round-trip would cost more for <20Mpx)
        cv::Mat img = cv::imread(path);
        if (img.empty()) {
            RCLCPP_ERROR(log, "Could not load %s image: %s", tag.c_str(), path.c_str());
            return;
        }
        cv::Mat gray;
        cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);

        // SIFT (each producer pinned to half the cores to avoid contention)
        cv::setNumThreads(half_cores);
        std::vector<cv::KeyPoint> kps;
        cv::Mat des;
        auto sift = cv::SIFT::create();
        sift->detectAndCompute(gray, cv::noArray(), kps, des);

        // Upload feature set to VRAM (single, one-time H2D transfer).
        auto gf = paralel::upload_features_to_gpu(
            des, kps, path, gray.rows, gray.cols);
        gf->publish_timestamp = now_epoch_s();

        // ── PUBLISH: hand the device pointers to the bus (no copy) ───────────
        bus_.publish(slot, gf);

        {
            std::lock_guard<std::mutex> lk(perf_mtx_);
            perf_->add_info(tag + "_keypoints", std::to_string(kps.size()));
            perf_->add_info(tag + "_size",
                std::to_string(img.cols) + "x" + std::to_string(img.rows));
        }
        RCLCPP_INFO(log, "[producer:%s] %zu keypoints uploaded to VRAM and published",
            tag.c_str(), kps.size());

        // Optional, off-critical-path keypoint visualization.
        if (save_vis_) {
            std::lock_guard<std::mutex> lk(vis_mtx_);
            cv::Mat vis;
            cv::drawKeypoints(img, kps, vis, cv::Scalar::all(-1),
                              cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
            cv::imwrite(out_dir_ + "/pipeline_" + tag + "_keypoints.png", vis);
        }
    }

    void run_once()
    {
        start_timer_->cancel();
        auto log = get_logger();

        // Warm up the CUDA context in the background while we set up producers.
        auto warmup = std::async(std::launch::async,
            [] { paralel::cuda_device_info(); });

        int total = std::max(1, (int)std::thread::hardware_concurrency());
        int half  = std::max(1, total / 2);

        perf_->mark_event("pipeline_started");
        warmup.wait();

        // ── Stage A: parallel produce (SIFT + upload + publish) ──────────────
        {
            auto s = perf_->begin_stage("A_parallel_sift_upload_publish");
            s->set_meta("threads_per_sift",
                std::to_string(half) + " of " + std::to_string(total));
            s->set_meta("note", "each set published to VRAM bus as soon as ready");

            std::thread t_query([&]{
                produce(query_path_, paralel::GpuFeatureBus::Slot::kQuery,
                        "query", half);
            });
            std::thread t_large([&]{
                produce(large_path_, paralel::GpuFeatureBus::Slot::kLarge,
                        "large", half);
            });
            t_query.join();
            t_large.join();
        }
        RCLCPP_INFO(log, "[A] SIFT + VRAM upload + publish: %.1f ms",
            perf_->stage_wall_ms("A_parallel_sift_upload_publish"));

        // ── Stage B: subscribe (wait for pair on the VRAM bus) ───────────────
        std::shared_ptr<paralel::GpuFeatures> q, l;
        {
            auto s = perf_->begin_stage("B_vram_bus_subscribe");
            auto pair = bus_.wait_for_pair();
            q = pair.first;
            l = pair.second;
            s->set_meta("query_kp", std::to_string(q ? q->num_keypoints : 0));
            s->set_meta("train_kp", std::to_string(l ? l->num_keypoints : 0));
            s->set_meta("note", "device-pointer hand-off, zero payload copy");
        }
        perf_->mark_event("features_received_from_vram_bus");

        if (!q || !l || !q->valid() || !l->valid()) {
            RCLCPP_ERROR(log, "Invalid feature sets received from VRAM bus");
            finish(false);
            return;
        }

        double lat_q = (now_epoch_s() - q->publish_timestamp) * 1000.0;
        double lat_l = (now_epoch_s() - l->publish_timestamp) * 1000.0;
        perf_->add_info("query_bus_latency_ms", std::to_string(lat_q).substr(0, 7));
        perf_->add_info("large_bus_latency_ms", std::to_string(lat_l).substr(0, 7));
        RCLCPP_INFO(log, "[B] Subscribed from VRAM bus (query %d kp, train %d kp)",
            q->num_keypoints, l->num_keypoints);

        // ── Stage C: GPU kNN matching + fused Lowe ratio test (on VRAM) ──────
        paralel::GpuMatchResult mr;
        {
            auto s = perf_->begin_stage("C_gpu_knn_match_ratio");
            s->set_meta("method",
                "GPU brute-force kNN (k=2, L2) + fused Lowe ratio (VRAM-resident)");
            mr = paralel::cuda_knn_match_ratio(*q, *l, 0.7f);
            s->set_meta("good_matches", std::to_string(mr.good_matches));
            if (mr.total_query > 0) {
                double rej = (1.0 - (double)mr.good_matches / mr.total_query) * 100.0;
                s->set_meta("rejection_rate",
                    std::to_string((int)rej) + "%");
            }
        }
        RCLCPP_INFO(log, "[C] GPU kNN + ratio test: %.1f ms (%d good matches)",
            perf_->stage_wall_ms("C_gpu_knn_match_ratio"), mr.good_matches);

        if (mr.good_matches < 4) {
            RCLCPP_ERROR(log, "Not enough good matches (%d < 4)", mr.good_matches);
            finish(false);
            return;
        }

        // ── Stage D: pull keypoint coords for matched pairs (small D2H) ──────
        // We only need the coordinates of the matched keypoints for homography.
        // These are tiny (a few hundred floats), so the transfer is negligible.
        std::vector<cv::Point2f> src_pts, dst_pts;
        std::vector<float> q_x, q_y, l_x, l_y;
        {
            auto s = perf_->begin_stage("D_collect_matched_points");
            q_x = download(q->d_kp_x, q->num_keypoints);
            q_y = download(q->d_kp_y, q->num_keypoints);
            l_x = download(l->d_kp_x, l->num_keypoints);
            l_y = download(l->d_kp_y, l->num_keypoints);

            src_pts.reserve(mr.good_matches);
            dst_pts.reserve(mr.good_matches);
            for (int i = 0; i < mr.good_matches; ++i) {
                int qi = mr.query_idx[i];
                int ti = mr.train_idx[i];
                src_pts.emplace_back(q_x[qi], q_y[qi]);
                dst_pts.emplace_back(l_x[ti], l_y[ti]);
            }
            s->set_meta("point_pairs", std::to_string(src_pts.size()));
        }

        // ── Stage E: homography (RANSAC) ─────────────────────────────────────
        cv::Mat M, mask;
        {
            auto s = perf_->begin_stage("E_homography_ransac");
            M = cv::findHomography(src_pts, dst_pts, cv::RANSAC, 5.0, mask);
            if (!mask.empty()) {
                int inliers = cv::countNonZero(mask);
                double pct = 100.0 * inliers / mr.good_matches;
                s->set_meta("inliers",
                    std::to_string(inliers) + "/" +
                    std::to_string(mr.good_matches) + " (" +
                    std::to_string((int)pct) + "%)");
            }
        }
        RCLCPP_INFO(log, "[E] Homography (RANSAC): %.1f ms",
            perf_->stage_wall_ms("E_homography_ransac"));

        if (M.empty()) {
            RCLCPP_ERROR(log, "Homography could not be computed.");
            finish(false);
            return;
        }

        // ── Stage F: perspective transform of query corners ──────────────────
        std::vector<cv::Point2f> dst_corners;
        {
            auto s = perf_->begin_stage("F_perspective_transform");
            int h = q->height, w = q->width;
            std::vector<cv::Point2f> corners = {
                {0.f, 0.f}, {0.f, (float)(h - 1)},
                {(float)(w - 1), (float)(h - 1)}, {(float)(w - 1), 0.f}
            };
            cv::perspectiveTransform(corners, dst_corners, M);
        }
        std::string corners_str;
        for (auto & c : dst_corners) {
            if (!corners_str.empty()) corners_str += "; ";
            corners_str += "(" + std::to_string(c.x).substr(0, 6) +
                           ", " + std::to_string(c.y).substr(0, 6) + ")";
        }
        perf_->add_info("detected_region_corners", corners_str);

        // ── Optional visualization (off critical path) ───────────────────────
        if (save_vis_) {
            auto s = perf_->begin_stage("G_save_result_visualization");
            cv::Mat large_img = cv::imread(l->image_name);
            if (!large_img.empty()) {
                std::vector<cv::Point> poly;
                for (auto & p : dst_corners) poly.emplace_back(cv::Point(p));
                cv::polylines(large_img, poly, true,
                              cv::Scalar(0, 255, 0), 3, cv::LINE_AA);
                std::string p = out_dir_ + "/pipeline_detected_region.png";
                cv::imwrite(p, large_img);
                cv::imwrite("matching_result.png", large_img);
                s->set_meta("file", p);
            }
        }

        perf_->add_info("good_matches_after_ratio_test", std::to_string(mr.good_matches));
        if (!mask.empty())
            perf_->add_info("ransac_inliers", std::to_string(cv::countNonZero(mask)));
        perf_->mark_event("pipeline_finished");

        finish(true);
    }

    static std::vector<float> download(const float * d_ptr, int n);

    void finish(bool success)
    {
        perf_->add_info("pipeline_success", success ? "true" : "false");
        // The ONLY thing that crosses back to the CPU for the user: perf numbers.
        perf_->log_report(get_logger());
        auto [txt, js] = perf_->save_report("pipeline_performance");
        RCLCPP_INFO(get_logger(),
            "Performance report written to %s and %s", txt.c_str(), js.c_str());
        RCLCPP_INFO(get_logger(), "SIFT pipeline shutting down.");
        rclcpp::shutdown();
    }

    // ── Members ──────────────────────────────────────────────────────────────
    std::string large_path_, query_path_, out_dir_;
    bool save_vis_ = true;

    std::unique_ptr<paralel::PerfTracker> perf_;
    std::mutex perf_mtx_, vis_mtx_;

    paralel::GpuFeatureBus bus_;
    rclcpp::TimerBase::SharedPtr start_timer_;
};

// download() needs cudaMemcpy; defined out-of-line so this .cpp doesn't pull
// in the full CUDA runtime header set in its class body. We declare it here and
// implement via the cuda_kernels translation unit instead would be cleaner, but
// a thin local copy keeps the dependency surface small.
#include <cuda_runtime.h>
std::vector<float> SiftPipeline::download(const float * d_ptr, int n)
{
    std::vector<float> h(n);
    if (n > 0 && d_ptr) {
        cudaMemcpy(h.data(), d_ptr, n * sizeof(float), cudaMemcpyDeviceToHost);
    }
    return h;
}

// ─────────────────────────────────────────────────────────────────────────────
int main(int argc, char ** argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<SiftPipeline>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
