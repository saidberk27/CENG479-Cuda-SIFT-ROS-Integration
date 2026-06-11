#!/usr/bin/env python3
"""
Parallel FLANN Subscriber — template node (hello-world).
This is a skeleton for the parallel implementation.
"""

import rclpy
from rclpy.node import Node


class FlannSubscriber(Node):
    """Template subscriber node for the parallel pipeline."""

    def __init__(self):
        super().__init__('flann_subscriber')
        self.get_logger().info('Hello from parallel FLANN subscriber (template)')

    def run(self):
        self.get_logger().info('Parallel FLANN subscriber is running (template).')


def main(args=None):
    rclpy.init(args=args)
    node = FlannSubscriber()
    rclpy.spin(node)
    rclpy.shutdown()


if __name__ == '__main__':
    main()