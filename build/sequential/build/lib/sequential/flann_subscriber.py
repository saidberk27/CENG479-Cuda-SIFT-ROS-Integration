#!/usr/bin/env python3

import os
import time

import cv2
import numpy as np

import rclpy
from rclpy.node import Node

from sift_interfaces.msg import ImageFeatures

from sequential.perf_utils import PerfTracker


# Default output directory: src/sequential/output relative to workspace root
_DEFAULT_OUTPUT = os.path.abspath(os.path.join(os.getcwd(), 'src', 'sequential', 'output'))


class FlannSubscriber(Node):
    """ROS2 node that receives SIFT features and performs FLANN matching.

    Detailed performance metrics are printed to the terminal AND saved to
    output/subscriber_performance.{txt,json}. Intermediate output images are
    saved to output/ after every processing stage (received keypoints, raw
    matches, ratio-test matches, RANSAC inliers, detected region, final
    composite figure).
    """

    def __init__(self):
        super().__init__('flann_subscriber')

        self.declare_parameter('output_dir', _DEFAULT_OUTPUT)
        self.out_dir = self.get_parameter('output_dir').get_parameter_value().string_value

        self.perf = PerfTracker('flann_subscriber', output_dir=self.out_dir)

        # Storage for received messages
        self._msg_large = None
        self._msg_query = None
        self._done = False

        # Subscribers
        self.sub_large = self.create_subscription(
            ImageFeatures, '/sift/large_features', self._cb_large, 10)
        self.sub_query = self.create_subscription(
            ImageFeatures, '/sift/query_features', self._cb_query, 10)

        # Timer to check if we have both messages
        self.create_timer(0.1, self._try_match)

        self.get_logger().info('FLANN subscriber waiting for SIFT features...')

    # ── Message callbacks ────────────────────────────────────────────
    def _cb_large(self, msg: ImageFeatures):
        recv_ms = self.perf.mark_event('received_large_features')
        transport_ms = (time.time() - msg.publish_timestamp) * 1000.0 \
            if msg.publish_timestamp > 0 else float('nan')
        payload_mb = (len(msg.descriptors) * 4 + msg.num_keypoints * 16) / 1024 / 1024
        self.perf.add_info('large_transport_latency_ms', f'{transport_ms:.3f}')
        self.perf.add_info('large_payload_mb', f'{payload_mb:.2f}')
        self.get_logger().info(
            f'Received large features: {msg.num_keypoints} keypoints, '
            f'~{payload_mb:.2f} MB payload, '
            f'transport latency {transport_ms:.2f} ms '
            f'(+{recv_ms:.1f} ms from node start)'
        )
        self._msg_large = msg

    def _cb_query(self, msg: ImageFeatures):
        recv_ms = self.perf.mark_event('received_query_features')
        transport_ms = (time.time() - msg.publish_timestamp) * 1000.0 \
            if msg.publish_timestamp > 0 else float('nan')
        payload_mb = (len(msg.descriptors) * 4 + msg.num_keypoints * 16) / 1024 / 1024
        self.perf.add_info('query_transport_latency_ms', f'{transport_ms:.3f}')
        self.perf.add_info('query_payload_mb', f'{payload_mb:.2f}')
        self.get_logger().info(
            f'Received query features: {msg.num_keypoints} keypoints, '
            f'~{payload_mb:.2f} MB payload, '
            f'transport latency {transport_ms:.2f} ms '
            f'(+{recv_ms:.1f} ms from node start)'
        )
        self._msg_query = msg

    # ── Reconstruction helpers ───────────────────────────────────────
    def _to_keypoints(self, msg: ImageFeatures):
        """Reconstruct cv2.KeyPoint list from ImageFeatures message."""
        kps = []
        for i in range(msg.num_keypoints):
            kp = cv2.KeyPoint(
                x=msg.keypoints_x[i],
                y=msg.keypoints_y[i],
                size=msg.keypoints_size[i],
                angle=msg.keypoints_angle[i],
            )
            kps.append(kp)
        return kps

    def _to_descriptors(self, msg: ImageFeatures):
        """Reconstruct descriptor numpy array from ImageFeatures message."""
        n = msg.num_keypoints
        if n == 0 or len(msg.descriptors) == 0:
            return None
        arr = np.array(msg.descriptors, dtype=np.float32)
        # SIFT descriptors are 128-dimensional
        return arr.reshape(n, 128)

    def _save_image(self, filename, image):
        path = os.path.join(self.out_dir, filename)
        cv2.imwrite(path, image)
        return path

    def _finish(self, success):
        """Print + save the final performance report, then shut down."""
        log = self.get_logger()
        self.perf.add_info('pipeline_success', success)
        self.perf.log_report(log)
        # Save the subscriber-only report (legacy, kept for reference)
        txt, js = self.perf.save_report('subscriber_performance')
        log.info(f'Performance report written to {txt} and {js}')
        # Save the unified pipeline report (same format as paralel)
        self._save_unified_report(success)
        log.info('FLANN subscriber shutting down.')
        rclpy.shutdown()

    def _save_unified_report(self, success):
        """Merge publisher + subscriber data into a single pipeline_performance.json
        that matches the format of src/paralel/output/pipeline_performance.json."""
        import json, platform
        log = self.get_logger()

        # Load publisher report if it exists
        pub_json = os.path.join(self.out_dir, 'publisher_performance.json')
        pub_data = {}
        if os.path.exists(pub_json):
            try:
                with open(pub_json) as f:
                    pub_data = json.load(f)
            except Exception:
                pass

        # Build unified stages: publisher stages first, then subscriber stages
        unified_stages = {}
        for name, rec in pub_data.get('stages', {}).items():
            unified_stages[f'pub_{name}'] = rec
        for name, rec in self.perf.stages.items():
            unified_stages[f'sub_{name}'] = {
                'wall_ms': rec['wall_ms'],
                'cpu_ms': rec['cpu_ms'],
                'started_at_ms': rec['started_at_ms'],
                'meta': rec['meta'],
            }

        # Unified events: publisher events + subscriber events
        unified_events = {}
        for k, v in pub_data.get('events_ms', {}).items():
            unified_events[f'pub_{k}'] = v
        for k, v in self.perf.events.items():
            unified_events[f'sub_{k}'] = v

        # Unified info: merge both
        unified_info = {}
        unified_info.update(pub_data.get('info', {}))
        unified_info.update(dict(self.perf.extra))
        unified_info['pipeline_success'] = str(success)

        # Total wall time = subscriber total (covers the full end-to-end from sub start)
        import time as _time
        sub_total_wall = (_time.perf_counter() - self.perf.start_perf) * 1000.0
        sub_total_cpu  = (_time.process_time() - self.perf.start_cpu) * 1000.0
        pub_total_wall = pub_data.get('total_wall_ms', 0.0)
        pub_total_cpu  = pub_data.get('total_cpu_ms', 0.0)

        payload = {
            'node': 'sequential_pipeline',
            'timestamp': self.perf.start_wall.isoformat(),
            'host': platform.node(),
            'language': 'Python 3 / OpenCV',
            'pid_publisher': pub_data.get('pid', None),
            'pid_subscriber': os.getpid(),
            'publisher_total_wall_ms': pub_total_wall,
            'publisher_total_cpu_ms': pub_total_cpu,
            'subscriber_total_wall_ms': sub_total_wall,
            'subscriber_total_cpu_ms': sub_total_cpu,
            'info': unified_info,
            'stages': unified_stages,
            'events_ms': unified_events,
        }

        out_path = os.path.join(self.out_dir, 'pipeline_performance.json')
        with open(out_path, 'w') as f:
            json.dump(payload, f, indent=2, default=str)
        log.info(f'Unified pipeline report written to {out_path}')


    # ── Main processing ──────────────────────────────────────────────
    def _try_match(self):
        if self._done or self._msg_large is None or self._msg_query is None:
            return  # Not ready yet
        self._done = True

        log = self.get_logger()
        perf = self.perf
        out = self.out_dir

        perf.mark_event('matching_pipeline_started')
        log.info('Both feature sets received. Starting FLANN matching pipeline...')

        # Try loading original images for per-stage visualization
        large_img = cv2.imread(self._msg_large.image_name)
        query_img = cv2.imread(self._msg_query.image_name)
        can_visualize = large_img is not None and query_img is not None
        if not can_visualize:
            log.warn('Could not load original images; stage images will be skipped')

        # ── Stage 1: Data reconstruction ─────────────────────────────
        with perf.stage('1_data_reconstruction') as rec:
            kp_large = self._to_keypoints(self._msg_large)
            kp_query = self._to_keypoints(self._msg_query)
            des_large = self._to_descriptors(self._msg_large)
            des_query = self._to_descriptors(self._msg_query)
            rec['meta']['large_keypoints'] = len(kp_large)
            rec['meta']['query_keypoints'] = len(kp_query)
        log.info(f'[1] Reconstructed {len(kp_large)} + {len(kp_query)} keypoints '
                 f'in {perf.stages["1_data_reconstruction"]["wall_ms"]:.1f} ms')

        if des_query is None or des_large is None:
            log.error('No descriptors to match')
            self._finish(success=False)
            return

        # Stage output image: received keypoints drawn on images
        if can_visualize:
            with perf.stage('1b_save_received_keypoints_image') as rec:
                vis_l = cv2.drawKeypoints(large_img, kp_large, None, color=(0, 255, 0))
                vis_q = cv2.drawKeypoints(query_img, kp_query, None, color=(0, 255, 0))
                p1 = self._save_image('subscriber_1_large_received_keypoints.png', vis_l)
                p2 = self._save_image('subscriber_2_query_received_keypoints.png', vis_q)
                rec['meta']['files'] = f'{p1}, {p2}'
            log.info(f'[1b] Saved received-keypoint images -> {p1}, {p2}')

        # ── Stage 2: FLANN index build + kNN matching ────────────────
        FLANN_INDEX_KDTREE = 1
        index_params = dict(algorithm=FLANN_INDEX_KDTREE, trees=5)
        search_params = dict(checks=50)

        with perf.stage('2_flann_index_build') as rec:
            flann = cv2.FlannBasedMatcher(index_params, search_params)
            rec['meta']['algorithm'] = 'KD-Tree (trees=5, checks=50)'

        with perf.stage('3_flann_knn_matching') as rec:
            matches = flann.knnMatch(des_query, des_large, k=2)
            rec['meta']['raw_matches'] = len(matches)
        wall = perf.stages['3_flann_knn_matching']['wall_ms']
        perf.add_stage_meta('3_flann_knn_matching', 'throughput',
                            f'{len(matches) / wall * 1000:.0f} matches/s' if wall > 0 else 'n/a')
        log.info(f'[3] FLANN kNN matching: {wall:.1f} ms ({len(matches)} raw matches)')

        # ── Stage 4: Lowe's ratio test ───────────────────────────────
        with perf.stage('4_lowes_ratio_test') as rec:
            good_matches = []
            for m, n in matches:
                if m.distance < 0.7 * n.distance:
                    good_matches.append(m)
            rec['meta']['good_matches'] = len(good_matches)
            rec['meta']['rejection_rate'] = (
                f'{(1 - len(good_matches) / len(matches)) * 100:.1f}%'
                if matches else 'n/a')
        log.info(f"[4] Lowe's ratio test: "
                 f'{perf.stages["4_lowes_ratio_test"]["wall_ms"]:.1f} ms '
                 f'({len(good_matches)} good matches, '
                 f'{perf.stages["4_lowes_ratio_test"]["meta"]["rejection_rate"]} rejected)')

        # Stage output image: good matches after ratio test
        if can_visualize:
            with perf.stage('4b_save_ratio_test_image') as rec:
                img_good = cv2.drawMatches(
                    query_img, kp_query, large_img, kp_large,
                    good_matches, None,
                    matchColor=(0, 255, 0),
                    flags=cv2.DrawMatchesFlags_NOT_DRAW_SINGLE_POINTS)
                p3 = self._save_image('subscriber_3_ratio_test_matches.png', img_good)
                rec['meta']['files'] = p3
            log.info(f'[4b] Saved ratio-test match image -> {p3}')

        if len(good_matches) < 4:
            log.error('Not enough good matches to compute homography.')
            self._finish(success=False)
            return

        # ── Stage 5: Point extraction ────────────────────────────────
        with perf.stage('5_point_extraction') as rec:
            src_pts = np.float32(
                [kp_query[m.queryIdx].pt for m in good_matches]).reshape(-1, 1, 2)
            dst_pts = np.float32(
                [kp_large[m.trainIdx].pt for m in good_matches]).reshape(-1, 1, 2)
            rec['meta']['point_pairs'] = len(good_matches)
        log.info(f'[5] Point extraction: '
                 f'{perf.stages["5_point_extraction"]["wall_ms"]:.2f} ms')

        # ── Stage 6: Homography (RANSAC) ─────────────────────────────
        with perf.stage('6_homography_ransac') as rec:
            M, mask = cv2.findHomography(src_pts, dst_pts, cv2.RANSAC, 5.0)
            if mask is not None:
                inliers = int(mask.sum())
                rec['meta']['inliers'] = (
                    f'{inliers}/{len(good_matches)} '
                    f'({inliers / len(good_matches) * 100:.1f}%)')
        log.info(f'[6] Homography (RANSAC): '
                 f'{perf.stages["6_homography_ransac"]["wall_ms"]:.1f} ms')

        if M is None:
            log.error('Homography could not be computed.')
            self._finish(success=False)
            return

        matches_mask = mask.ravel().tolist()
        inlier_matches = [m for m, keep in zip(good_matches, matches_mask) if keep]
        log.info(f'    RANSAC inliers: {len(inlier_matches)}/{len(good_matches)}')

        # ── Stage 7: Perspective transform of query corners ──────────
        with perf.stage('7_perspective_transform') as rec:
            h = self._msg_query.height
            w = self._msg_query.width
            pts = np.float32(
                [[0, 0], [0, h - 1], [w - 1, h - 1], [w - 1, 0]]).reshape(-1, 1, 2)
            dst = cv2.perspectiveTransform(pts, M)
        log.info(f'[7] Perspective transform: '
                 f'{perf.stages["7_perspective_transform"]["wall_ms"]:.2f} ms')

        # Print matching region corners
        log.info('Matching region corners on large image:')
        corners = dst.reshape(4, 2)
        for i, pt in enumerate(corners):
            log.info(f'  Corner {i + 1}: ({pt[0]:.1f}, {pt[1]:.1f})')
        perf.add_info('detected_region_corners',
                      '; '.join(f'({p[0]:.1f}, {p[1]:.1f})' for p in corners))

        # Stage output images: inlier matches + detected region
        if can_visualize:
            with perf.stage('7b_save_ransac_stage_images') as rec:
                # Inlier-only match visualization
                img_inliers = cv2.drawMatches(
                    query_img, kp_query, large_img, kp_large,
                    good_matches, None,
                    matchColor=(0, 255, 0),
                    singlePointColor=None,
                    matchesMask=matches_mask,
                    flags=cv2.DrawMatchesFlags_NOT_DRAW_SINGLE_POINTS)
                p4 = self._save_image('subscriber_4_ransac_inlier_matches.png', img_inliers)

                # Detected region on large image
                large_with_box = large_img.copy()
                cv2.polylines(large_with_box, [np.int32(dst)], True,
                              (0, 255, 0), 3, cv2.LINE_AA)
                p5 = self._save_image('subscriber_5_detected_region.png', large_with_box)
                rec['meta']['files'] = f'{p4}, {p5}'
            log.info(f'[7b] Saved RANSAC stage images -> {p4}, {p5}')

        # ── Stage 8: Final composite visualization ───────────────────
        if can_visualize:
            try:
                with perf.stage('8_final_composite_figure') as rec:
                    import matplotlib
                    matplotlib.use('Agg')
                    import matplotlib.pyplot as plt

                    plt.figure(figsize=(20, 10))

                    plt.subplot(221)
                    plt.imshow(cv2.cvtColor(query_img, cv2.COLOR_BGR2RGB))
                    plt.title("Query Image")
                    plt.axis("off")

                    plt.subplot(222)
                    plt.imshow(cv2.cvtColor(large_img, cv2.COLOR_BGR2RGB))
                    plt.title("Train Image")
                    plt.axis("off")

                    plt.subplot(223)
                    plt.imshow(cv2.cvtColor(large_with_box, cv2.COLOR_BGR2RGB))
                    plt.title("Detected Region on Large Image")
                    plt.axis("off")

                    plt.subplot(224)
                    plt.imshow(cv2.cvtColor(img_inliers, cv2.COLOR_BGR2RGB))
                    plt.title("SIFT Feature Matches (RANSAC inliers)")
                    plt.axis("off")

                    plt.tight_layout()
                    p6 = os.path.join(out, 'subscriber_6_matching_result.png')
                    plt.savefig(p6, dpi=150)
                    plt.close()
                    rec['meta']['files'] = p6
                log.info(f'[8] Final composite figure saved -> {p6} '
                         f'({perf.stages["8_final_composite_figure"]["wall_ms"]:.1f} ms)')

                # Legacy output path for backwards compatibility
                import shutil
                shutil.copyfile(p6, 'matching_result.png')
                log.info('    (also copied to matching_result.png)')
            except ImportError:
                log.warn('matplotlib not available, skipping composite figure')

        # ── Final summary info + report ──────────────────────────────
        perf.add_info('raw_matches', len(matches))
        perf.add_info('good_matches_after_ratio_test', len(good_matches))
        perf.add_info('ransac_inliers', len(inlier_matches))
        perf.mark_event('matching_pipeline_finished')

        self._finish(success=True)


def main(args=None):
    rclpy.init(args=args)
    node = FlannSubscriber()
    rclpy.spin(node)


if __name__ == '__main__':
    main()