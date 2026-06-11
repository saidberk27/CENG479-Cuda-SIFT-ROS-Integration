#pragma once
/**
 * perf_tracker.hpp
 * ─────────────────────────────────────────────────────────────────────────────
 * C++ performance-measurement utility for the paralel CUDA ROS2 pipeline.
 * Mirrors the Python PerfTracker in src/sequential/sequential/perf_utils.py.
 *
 * Features:
 *   • Named stages timed with std::chrono (wall) + clock_gettime CLOCK_PROCESS_CPUTIME_ID (CPU)
 *   • Per-stage metadata (string key/value pairs)
 *   • Point-in-time events (ms since tracker creation)
 *   • /proc-based memory / resource snapshot (Linux)
 *   • Formatted terminal report (via rclcpp logger)
 *   • Saves .txt and .json reports to disk
 */

#include <chrono>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <sys/resource.h>
#include <unistd.h>
#include <vector>

#include "rclcpp/rclcpp.hpp"

namespace paralel {

// ─────────────────────────────────────────────────────────────────────────────
// StageRecord
// ─────────────────────────────────────────────────────────────────────────────
struct StageRecord {
    double wall_ms{0.0};
    double cpu_ms{0.0};
    double started_at_ms{0.0};
    std::map<std::string, std::string> meta;
};

// ─────────────────────────────────────────────────────────────────────────────
// ScopedStage  –  RAII timer for a single stage
// ─────────────────────────────────────────────────────────────────────────────
class ScopedStage {
public:
    ScopedStage(const std::string & name,
                std::map<std::string, StageRecord> & stages,
                double tracker_start_wall,
                double tracker_start_cpu)
    : name_(name), stages_(stages),
      tracker_start_wall_(tracker_start_wall),
      tracker_start_cpu_(tracker_start_cpu)
    {
        t0_wall_ = now_wall_ms();
        t0_cpu_  = now_cpu_ms();
        record_.started_at_ms = t0_wall_ - tracker_start_wall_;
    }

    ~ScopedStage() {
        record_.wall_ms = now_wall_ms() - t0_wall_;
        record_.cpu_ms  = now_cpu_ms()  - t0_cpu_;
        stages_[name_]  = record_;
    }

    // Allow caller to add metadata while the stage is running
    void set_meta(const std::string & key, const std::string & value) {
        record_.meta[key] = value;
    }

    StageRecord & record() { return record_; }

private:
    static double now_wall_ms() {
        using namespace std::chrono;
        return duration<double, std::milli>(
            steady_clock::now().time_since_epoch()).count();
    }
    static double now_cpu_ms() {
        struct timespec ts{};
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ts);
        return ts.tv_sec * 1000.0 + ts.tv_nsec / 1e6;
    }

    std::string name_;
    std::map<std::string, StageRecord> & stages_;
    double tracker_start_wall_;
    double tracker_start_cpu_;
    double t0_wall_{0.0};
    double t0_cpu_{0.0};
    StageRecord record_;
};

// ─────────────────────────────────────────────────────────────────────────────
// PerfTracker
// ─────────────────────────────────────────────────────────────────────────────
class PerfTracker {
public:
    explicit PerfTracker(const std::string & node_name,
                         const std::string & output_dir = "output")
    : node_name_(node_name), output_dir_(output_dir)
    {
        start_wall_ms_ = now_wall_ms();
        start_cpu_ms_  = now_cpu_ms();

        // Record human-readable start time
        std::time_t t = std::time(nullptr);
        char buf[64];
        std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&t));
        start_wall_str_ = buf;

        // Ensure output directory exists
        std::string cmd = "mkdir -p " + output_dir_;
        (void)std::system(cmd.c_str());
    }

    // ── Stage API ────────────────────────────────────────────────────────────
    /** Begin a timed stage; returns a ScopedStage whose destructor stops the timer. */
    std::unique_ptr<ScopedStage> begin_stage(const std::string & name) {
        // Preserve insertion order via ordered_stage_names_
        if (stages_.find(name) == stages_.end()) {
            ordered_stage_names_.push_back(name);
        }
        return std::make_unique<ScopedStage>(
            name, stages_, start_wall_ms_, start_cpu_ms_);
    }

    /** Record a point-in-time event (ms since tracker creation). */
    double mark_event(const std::string & name) {
        double ms = now_wall_ms() - start_wall_ms_;
        events_[name] = ms;
        ordered_event_names_.push_back(name);
        return ms;
    }

    void add_info(const std::string & key, const std::string & value) {
        extra_[key] = value;
        ordered_extra_keys_.push_back(key);
    }

    void add_stage_meta(const std::string & stage,
                        const std::string & key,
                        const std::string & value) {
        if (stages_.count(stage)) {
            stages_[stage].meta[key] = value;
        }
    }

    double stage_wall_ms(const std::string & name) const {
        auto it = stages_.find(name);
        return it != stages_.end() ? it->second.wall_ms : 0.0;
    }

    // ── Reporting ────────────────────────────────────────────────────────────
    std::vector<std::string> build_report_lines() const {
        double total_wall_ms = now_wall_ms() - start_wall_ms_;
        double total_cpu_ms  = now_cpu_ms()  - start_cpu_ms_;

        const int W = 78;
        std::string bar(W, '=');
        std::string thin(W, '-');

        std::vector<std::string> lines;
        auto push = [&](const std::string & s){ lines.push_back(s); };

        push(bar);
        push("  PERFORMANCE REPORT  --  " + node_name_);
        push(bar);
        push("  Date/time        : " + start_wall_str_);
        push("  Host             : " + hostname());
        push("  PID              : " + std::to_string(getpid()));
        push("  Language         : C++17 / CUDA");

        if (!extra_.empty()) {
            push(thin);
            push("  GENERAL INFO");
            push(thin);
            for (auto & k : ordered_extra_keys_) {
                auto it = extra_.find(k);
                if (it != extra_.end()) {
                    push("  " + pad_right(k, 32) + ": " + it->second);
                }
            }
        }

        // Stage table
        push(thin);
        push("  STAGE TIMINGS");
        push(thin);
        push("  " + pad_right("Stage", 34) +
             pad_left("Wall (ms)", 11) +
             pad_left("CPU (ms)", 11) +
             pad_left("% wall", 9));
        push(thin);

        double stage_total = 0.0;
        for (auto & name : ordered_stage_names_) {
            auto it = stages_.find(name);
            if (it == stages_.end()) continue;
            const auto & rec = it->second;
            double pct = total_wall_ms > 0 ? rec.wall_ms / total_wall_ms * 100.0 : 0.0;
            stage_total += rec.wall_ms;
            push("  " + pad_right(name, 34) +
                 fmt_f(rec.wall_ms, 11, 3) +
                 fmt_f(rec.cpu_ms,  11, 3) +
                 fmt_f(pct, 8, 2) + "%");
            for (auto & [mk, mv] : rec.meta) {
                push("      - " + mk + ": " + mv);
            }
        }
        push(thin);
        double pct_sum = total_wall_ms > 0 ? stage_total / total_wall_ms * 100.0 : 0.0;
        push("  " + pad_right("Sum of measured stages", 34) +
             fmt_f(stage_total, 11, 3) + std::string(11, ' ') +
             fmt_f(pct_sum, 8, 2) + "%");
        push("  " + pad_right("TOTAL (since node start)", 34) +
             fmt_f(total_wall_ms, 11, 3) +
             fmt_f(total_cpu_ms,  11, 3) +
             fmt_f(100.0, 8, 2) + "%");

        // Events
        if (!events_.empty()) {
            push(thin);
            push("  TIMELINE EVENTS (ms since node start)");
            push(thin);
            for (auto & name : ordered_event_names_) {
                auto it = events_.find(name);
                if (it != events_.end()) {
                    push("  " + pad_right(name, 44) + fmt_f(it->second, 11, 3) + " ms");
                }
            }
        }

        // Resources
        auto res = resource_snapshot();
        push(thin);
        push("  PROCESS RESOURCE USAGE");
        push(thin);
        push("  Peak RSS memory                   : " + res.at("peak_rss_mb") + " MB");
        push("  User CPU time                     : " + res.at("user_cpu_s") + " s");
        push("  System CPU time                   : " + res.at("system_cpu_s") + " s");
        push("  Context switches (vol/invol)      : " +
             res.at("vol_ctx") + " / " + res.at("invol_ctx"));
        push("  Page faults (minor/major)         : " +
             res.at("minor_faults") + " / " + res.at("major_faults"));
        push(bar);

        return lines;
    }

    void log_report(rclcpp::Logger logger) const {
        auto lines = build_report_lines();
        std::string out = "\n";
        for (auto & l : lines) out += l + "\n";
        RCLCPP_INFO(logger, "%s", out.c_str());
    }

    /** Save .txt and .json reports. Returns {txt_path, json_path}. */
    std::pair<std::string, std::string> save_report(const std::string & basename) const {
        double total_wall_ms = now_wall_ms() - start_wall_ms_;
        double total_cpu_ms  = now_cpu_ms()  - start_cpu_ms_;

        // ── .txt ─────────────────────────────────────────────────────────────
        std::string txt_path = output_dir_ + "/" + basename + ".txt";
        {
            std::ofstream f(txt_path);
            for (auto & l : build_report_lines()) f << l << "\n";
        }

        // ── .json ────────────────────────────────────────────────────────────
        std::string json_path = output_dir_ + "/" + basename + ".json";
        {
            std::ofstream f(json_path);
            f << std::fixed << std::setprecision(6);
            f << "{\n";
            f << "  \"node\": \"" << node_name_ << "\",\n";
            f << "  \"timestamp\": \"" << start_wall_str_ << "\",\n";
            f << "  \"host\": \"" << hostname() << "\",\n";
            f << "  \"language\": \"C++17/CUDA\",\n";
            f << "  \"pid\": " << getpid() << ",\n";
            f << "  \"total_wall_ms\": " << total_wall_ms << ",\n";
            f << "  \"total_cpu_ms\": " << total_cpu_ms << ",\n";

            // info
            f << "  \"info\": {\n";
            bool first = true;
            for (auto & k : ordered_extra_keys_) {
                auto it = extra_.find(k);
                if (it == extra_.end()) continue;
                if (!first) f << ",\n";
                f << "    \"" << json_escape(k) << "\": \"" << json_escape(it->second) << "\"";
                first = false;
            }
            f << "\n  },\n";

            // stages
            f << "  \"stages\": {\n";
            first = true;
            for (auto & name : ordered_stage_names_) {
                auto it = stages_.find(name);
                if (it == stages_.end()) continue;
                const auto & rec = it->second;
                if (!first) f << ",\n";
                f << "    \"" << json_escape(name) << "\": {\n";
                f << "      \"wall_ms\": " << rec.wall_ms << ",\n";
                f << "      \"cpu_ms\": " << rec.cpu_ms << ",\n";
                f << "      \"started_at_ms\": " << rec.started_at_ms << ",\n";
                f << "      \"meta\": {";
                bool fm = true;
                for (auto & [mk, mv] : rec.meta) {
                    if (!fm) f << ", ";
                    f << "\"" << json_escape(mk) << "\": \"" << json_escape(mv) << "\"";
                    fm = false;
                }
                f << "}\n    }";
                first = false;
            }
            f << "\n  },\n";

            // events
            f << "  \"events_ms\": {\n";
            first = true;
            for (auto & name : ordered_event_names_) {
                auto it = events_.find(name);
                if (it == events_.end()) continue;
                if (!first) f << ",\n";
                f << "    \"" << json_escape(name) << "\": " << it->second;
                first = false;
            }
            f << "\n  }\n}\n";
        }

        return {txt_path, json_path};
    }

private:
    // ── Helpers ──────────────────────────────────────────────────────────────
    static double now_wall_ms() {
        using namespace std::chrono;
        return duration<double, std::milli>(
            steady_clock::now().time_since_epoch()).count();
    }
    static double now_cpu_ms() {
        struct timespec ts{};
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ts);
        return ts.tv_sec * 1000.0 + ts.tv_nsec / 1e6;
    }

    static std::string hostname() {
        char buf[256] = {};
        gethostname(buf, sizeof(buf));
        return buf;
    }

    static std::map<std::string, std::string> resource_snapshot() {
        struct rusage ru{};
        getrusage(RUSAGE_SELF, &ru);
        std::map<std::string, std::string> m;
        m["peak_rss_mb"]   = fmt_f(ru.ru_maxrss / 1024.0, 0, 2);
        m["user_cpu_s"]    = fmt_f(ru.ru_utime.tv_sec + ru.ru_utime.tv_usec / 1e6, 0, 3);
        m["system_cpu_s"]  = fmt_f(ru.ru_stime.tv_sec + ru.ru_stime.tv_usec / 1e6, 0, 3);
        m["vol_ctx"]       = std::to_string(ru.ru_nvcsw);
        m["invol_ctx"]     = std::to_string(ru.ru_nivcsw);
        m["minor_faults"]  = std::to_string(ru.ru_minflt);
        m["major_faults"]  = std::to_string(ru.ru_majflt);
        return m;
    }

    static std::string fmt_f(double v, int width, int prec) {
        std::ostringstream ss;
        if (width > 0) ss << std::setw(width);
        ss << std::fixed << std::setprecision(prec) << v;
        return ss.str();
    }

    static std::string pad_right(const std::string & s, int w) {
        if ((int)s.size() >= w) return s;
        return s + std::string(w - s.size(), ' ');
    }

    static std::string pad_left(const std::string & s, int w) {
        if ((int)s.size() >= w) return s;
        return std::string(w - s.size(), ' ') + s;
    }

    static std::string json_escape(const std::string & s) {
        std::string out;
        for (char c : s) {
            if (c == '"')  out += "\\\"";
            else if (c == '\\') out += "\\\\";
            else if (c == '\n') out += "\\n";
            else out += c;
        }
        return out;
    }

    // ── Members ──────────────────────────────────────────────────────────────
    std::string node_name_;
    std::string output_dir_;
    std::string start_wall_str_;
    double start_wall_ms_{0.0};
    double start_cpu_ms_{0.0};

    std::map<std::string, StageRecord> stages_;
    std::vector<std::string> ordered_stage_names_;

    std::map<std::string, double> events_;
    std::vector<std::string> ordered_event_names_;

    std::map<std::string, std::string> extra_;
    std::vector<std::string> ordered_extra_keys_;
};

// ─────────────────────────────────────────────────────────────────────────────
// Convenience macro: time a block and store the ScopedStage in a local var
//   PERF_STAGE(tracker, "stage_name") { ... }
// ─────────────────────────────────────────────────────────────────────────────
#define PERF_STAGE(tracker, name) \
    for (auto _stage_ = (tracker).begin_stage(name); _stage_; _stage_.reset())

} // namespace paralel
