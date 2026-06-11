#!/usr/bin/env python3
"""
Parallel SIFT Publisher — template node (hello-world).
This is a skeleton for the parallel implementation.
"""

import rclpy
from rclpy.node import Node


class SiftPublisher(Node):
    """Template publisher node for the parallel pipeline."""

    def __init__(self):
        super().__init__('sift_publisher')
        self.get_logger().info('Hello from parallel SIFT publisher (template)')

    def run(self):
        self.get_logger().info('Parallel SIFT publisher is running (template).')


def main(args=None):
    rclpy.init(args=args)
    node = SiftPublisher()
    rclpy.spin(node)
    rclpy.shutdown()


if __name__ == '__main__':
    main()