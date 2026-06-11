#!/usr/bin/env python3

from launch import LaunchDescription
from launch_ros.actions import Node


def generate_launch_description():
    return LaunchDescription([
        Node(
            package='sequential',
            executable='sift_publisher',
            name='sift_publisher',
            output='screen',
            parameters=[
                {'large_image': 'ankara.jpg'},
                {'query_image': 'test.jpg'},
            ],
        ),
        Node(
            package='sequential',
            executable='flann_subscriber',
            name='flann_subscriber',
            output='screen',
        ),
    ])