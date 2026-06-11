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


class SiftPublisher(Node):
    """ROS2 node that loads images, computes SIFT features, and publishes them.

    Detailed performance metrics are printed to the terminal AND saved to
    output/publisher_performance.{txt,json}. Intermediate output images
    (grayscale conversions and keypoint visualizations) are saved to output/.
    """

    def __init__(self):
        super().__init__('sift_publisher')

        # Declare parameters with defaults
        self.declare_parameter('large_image', 'ankara.jpg')
        self.declare_parameter('query_image', 'test.jpg')
        self.declare_parameter('output_dir', _DEFAULT_OUTPUT)

        large_path = self.get_parameter('large_image').get_parameter_value().string_value
        query_path = self.get_parameter('query_image').get_parameter_value().string_value
        out_dir = self.get_parameter('output_dir').get_parameter_value().string_value

        self.perf = PerfTracker('sift_publisher', output_dir=out_dir)
        self.out_dir = out_dir
        log = self.get_logger()

        self.perf.add_info('large_image_path', large_path)
        self.perf.add_info('query_image_path', query_path)

        # Publishers
        self.pub_large = self.create_publisher(ImageFeatures, '/sift/large_features', 10)
        self.pub_query = self.create_publisher(ImageFeatures, '/sift/query_features', 10)

        # ── Stage 1: Image loading ───────────────────────────────────
        with self.perf.stage('1_image_loading') as rec:
            large_img = cv2.imread(large_path)
            query_img = cv2.imread(query_path)

        if large_img is None:
            log.error(f'Could not load large image: {large_path}')
            raise RuntimeError(f'Failed to load {large_path}')
        if query_img is None:
            log.error(f'Could not load query image: {query_path}')
            raise RuntimeError(f'Failed to load {query_path}')

        large_mb = large_img.nbytes / 1024 / 1024
        query_mb = query_img.nbytes / 1024 / 1024
        self.perf.add_stage_meta('1_image_loading', 'large_image',
                                 f'{large_img.shape[1]}x{large_img.shape[0]} ({large_mb:.2f} MB in memory)')
        self.perf.add_stage_meta('1_image_loading', 'query_image',
                                 f'{query_img.shape[1]}x{query_img.shape[0]} ({query_mb:.2f} MB in memory)')
        log.info(f'[1] Images loaded in {self.perf.stages["1_image_loading"]["wall_ms"]:.1f} ms  '
                 f'(large: {large_img.shape[1]}x{large_img.shape[0]}, '
                 f'query: {query_img.shape[1]}x{query_img.shape[0]})')

        # ── Stage 2: Grayscale conversion ────────────────────────────
        with self.perf.stage('2_grayscale_conversion'):
            large_gray = cv2.cvtColor(large_img, cv2.COLOR_BGR2GRAY)
            query_gray = cv2.cvtColor(query_img, cv2.COLOR_BGR2GRAY)
        log.info(f'[2] Grayscale conversion in '
                 f'{self.perf.stages["2_grayscale_conversion"]["wall_ms"]:.1f} ms')

        # Output images after grayscale stage
        with self.perf.stage('2b_save_grayscale_images') as rec:
            p1 = os.path.join(out_dir, 'publisher_1_large_grayscale.png')
            p2 = os.path.join(out_dir, 'publisher_2_query_grayscale.png')
            cv2.imwrite(p1, large_gray)
            cv2.imwrite(p2, query_gray)
            rec['meta']['files'] = f'{p1}, {p2}'
        log.info(f'[2b] Saved grayscale images -> {p1}, {p2}')

        # ── Stage 3: SIFT feature extraction ─────────────────────────
        sift = cv2.SIFT_create()

        with self.perf.stage('3_sift_large_image') as rec:
            kp_large, des_large = sift.detectAndCompute(large_gray, None)
        n_px = large_gray.shape[0] * large_gray.shape[1]
        wall = self.perf.stages['3_sift_large_image']['wall_ms']
        self.perf.add_stage_meta('3_sift_large_image', 'keypoints', len(kp_large))
        self.perf.add_stage_meta('3_sift_large_image', 'throughput',
                                 f'{n_px / wall / 1000:.2f} Mpx/s' if wall > 0 else 'n/a')
        log.info(f'[3] SIFT (large) in {wall:.1f} ms -> {len(kp_large)} keypoints')

        with self.perf.stage('4_sift_query_image') as rec:
            kp_query, des_query = sift.detectAndCompute(query_gray, None)
        n_px = query_gray.shape[0] * query_gray.shape[1]
        wall = self.perf.stages['4_sift_query_image']['wall_ms']
        self.perf.add_stage_meta('4_sift_query_image', 'keypoints', len(kp_query))
        self.perf.add_stage_meta('4_sift_query_image', 'throughput',
                                 f'{n_px / wall / 1000:.2f} Mpx/s' if wall > 0 else 'n/a')
        log.info(f'[4] SIFT (query) in {wall:.1f} ms -> {len(kp_query)} keypoints')

        # Output images after SIFT stage: keypoint visualizations
        with self.perf.stage('4b_save_keypoint_images') as rec:
            vis_large = cv2.drawKeypoints(
                large_img, kp_large, None,
                flags=cv2.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS)
            vis_query = cv2.drawKeypoints(
                query_img, kp_query, None,
                flags=cv2.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS)
            p3 = os.path.join(out_dir, 'publisher_3_large_keypoints.png')
            p4 = os.path.join(out_dir, 'publisher_4_query_keypoints.png')
            cv2.imwrite(p3, vis_large)
            cv2.imwrite(p4, vis_query)
            rec['meta']['files'] = f'{p3}, {p4}'
        log.info(f'[4b] Saved keypoint visualizations -> {p3}, {p4}')

        # ── Stage 5: Message serialization (build ROS messages) ──────
        with self.perf.stage('5_message_serialization') as rec:
            self._msg_large = self._build_feature_msg(
                image_name=large_path,
                height=large_gray.shape[0],
                width=large_gray.shape[1],
                keypoints=kp_large,
                descriptors=des_large,
            )
            self._msg_query = self._build_feature_msg(
                image_name=query_path,
                height=query_gray.shape[0],
                width=query_gray.shape[1],
                keypoints=kp_query,
                descriptors=des_query,
            )
        large_payload_mb = (len(self._msg_large.descriptors) * 4 +
                            self._msg_large.num_keypoints * 16) / 1024 / 1024
        query_payload_mb = (len(self._msg_query.descriptors) * 4 +
                            self._msg_query.num_keypoints * 16) / 1024 / 1024
        self.perf.add_stage_meta('5_message_serialization', 'large_payload',
                                 f'~{large_payload_mb:.2f} MB ({self._msg_large.num_keypoints} kp)')
        self.perf.add_stage_meta('5_message_serialization', 'query_payload',
                                 f'~{query_payload_mb:.2f} MB ({self._msg_query.num_keypoints} kp)')
        log.info(f'[5] Messages built in '
                 f'{self.perf.stages["5_message_serialization"]["wall_ms"]:.1f} ms  '
                 f'(payloads: large ~{large_payload_mb:.2f} MB, query ~{query_payload_mb:.2f} MB)')

        # Publish after a short delay to let subscriber connect
        self._pub_timer = self.create_timer(0.5, self._publish_and_shutdown)

    def _build_feature_msg(self, image_name, height, width,
                           keypoints, descriptors):
        msg = ImageFeatures()
        msg.image_name = image_name
        msg.height = height
        msg.width = width
        msg.num_keypoints = len(keypoints)

        msg.keypoints_x = [kp.pt[0] for kp in keypoints]
        msg.keypoints_y = [kp.pt[1] for kp in keypoints]
        msg.keypoints_size = [kp.size for kp in keypoints]
        msg.keypoints_angle = [kp.angle for kp in keypoints]

        if descriptors is not None:
            msg.descriptors = descriptors.flatten().tolist()
        else:
            msg.descriptors = []

        return msg

    def _publish_and_shutdown(self):
        # Destroy timer so it only fires once
        self.destroy_timer(self._pub_timer)
        log = self.get_logger()

        # ── Stage 6: Publishing ──────────────────────────────────────
        with self.perf.stage('6_publish_messages') as rec:
            # Publish the smaller query message first so it arrives quickly,
            # then the large message (62 MB payload takes time over DDS).
            self._msg_query.publish_timestamp = time.time()
            self.pub_query.publish(self._msg_query)
            self.get_logger().info('Published query features, now publishing large features...')
            self._msg_large.publish_timestamp = time.time()
            self.pub_large.publish(self._msg_large)
            rec['meta']['topics'] = '/sift/large_features, /sift/query_features'
        self.perf.mark_event('messages_published')
        log.info(f'[6] Published large ({self._msg_large.num_keypoints} kp) and '
                 f'query ({self._msg_query.num_keypoints} kp) features in '
                 f'{self.perf.stages["6_publish_messages"]["wall_ms"]:.2f} ms')

        # ── Final report: terminal + files ───────────────────────────
        self.perf.log_report(log)
        txt, js = self.perf.save_report('publisher_performance')
        log.info(f'Performance report written to {txt} and {js}')

        # Shutdown after a short grace period so the messages are delivered
        self.create_timer(2.0, lambda: (
            log.info('SIFT publisher shutting down'),
            rclpy.try_shutdown()
        ))


def main(args=None):
    rclpy.init(args=args)
    node = SiftPublisher()
    rclpy.spin(node)


if __name__ == '__main__':
    main()