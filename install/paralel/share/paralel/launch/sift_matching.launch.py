#!/usr/bin/env python3
"""
Launch file for the paralel CUDA C++ SIFT/FLANN pipeline.

Starts a SINGLE fused node:
  - sift_pipeline  (SIFT producer + FLANN/match consumer in one process)

Why one node?
  The producer and consumer share the same address space and, crucially, the
  same GPU memory.  The SIFT feature sets live in VRAM and are "published" to
  the consumer as device pointers through an in-process bus — no sockets and no
  ROS-topic serialization of the heavy descriptor payload.  Only the final
  performance numbers ever come back to the CPU.

Usage:
  ros2 launch paralel sift_matching.launch.py
  ros2 launch paralel sift_matching.launch.py large_image:=/path/to/train.jpg query_image:=/path/to/query.jpg
"""

from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node


def generate_launch_description():
    large_image_arg = DeclareLaunchArgument(
        'large_image',
        default_value='ankara.jpg',
        description='Path to the large/train image'
    )
    query_image_arg = DeclareLaunchArgument(
        'query_image',
        default_value='test.jpg',
        description='Path to the query image'
    )
    save_vis_arg = DeclareLaunchArgument(
        'save_visualizations',
        default_value='true',
        description='Save keypoint / result images (off the critical path)'
    )

    sift_pipeline_node = Node(
        package='paralel',
        executable='sift_pipeline',
        name='sift_pipeline',
        output='screen',
        parameters=[
            {'large_image': LaunchConfiguration('large_image')},
            {'query_image': LaunchConfiguration('query_image')},
            {'save_visualizations': LaunchConfiguration('save_visualizations')},
        ],
    )

    return LaunchDescription([
        large_image_arg,
        query_image_arg,
        save_vis_arg,
        sift_pipeline_node,
    ])
