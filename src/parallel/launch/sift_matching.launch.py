#!/usr/bin/env python3

from launch import LaunchDescription
from launch_ros.actions import Node


def generate_launch_description():
    return LaunchDescription([
        Node(
            package='parallel',
            executable='sift_publisher',
            name='sift_publisher',
            output='screen',
        ),
        Node(
            package='parallel',
            executable='flann_subscriber',
            name='flann_subscriber',
            output='screen',
        ),
    ])