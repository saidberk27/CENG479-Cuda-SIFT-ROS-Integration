// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from sift_interfaces:msg/ImageFeatures.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "sift_interfaces/msg/image_features.h"


#ifndef SIFT_INTERFACES__MSG__DETAIL__IMAGE_FEATURES__STRUCT_H_
#define SIFT_INTERFACES__MSG__DETAIL__IMAGE_FEATURES__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// Constants defined in the message

// Include directives for member types
// Member 'image_name'
#include "rosidl_runtime_c/string.h"
// Member 'keypoints_x'
// Member 'keypoints_y'
// Member 'keypoints_size'
// Member 'keypoints_angle'
// Member 'descriptors'
#include "rosidl_runtime_c/primitives_sequence.h"

/// Struct defined in msg/ImageFeatures in the package sift_interfaces.
/**
  * Image meta information
 */
typedef struct sift_interfaces__msg__ImageFeatures
{
  rosidl_runtime_c__String image_name;
  int32_t height;
  int32_t width;
  /// Keypoints (n keypoints)
  /// Each keypoint has: x, y, size, angle
  rosidl_runtime_c__float__Sequence keypoints_x;
  rosidl_runtime_c__float__Sequence keypoints_y;
  rosidl_runtime_c__float__Sequence keypoints_size;
  rosidl_runtime_c__float__Sequence keypoints_angle;
  /// Descriptors flattened: n x 128 floats
  /// Row-major order: descriptor_0[0..127], descriptor_1[0..127], ...
  rosidl_runtime_c__float__Sequence descriptors;
  /// Number of keypoints
  int32_t num_keypoints;
  /// Publish timestamp (seconds since epoch, set right before publishing)
  /// Used by the subscriber to measure transport latency
  double publish_timestamp;
} sift_interfaces__msg__ImageFeatures;

// Struct for a sequence of sift_interfaces__msg__ImageFeatures.
typedef struct sift_interfaces__msg__ImageFeatures__Sequence
{
  sift_interfaces__msg__ImageFeatures * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} sift_interfaces__msg__ImageFeatures__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // SIFT_INTERFACES__MSG__DETAIL__IMAGE_FEATURES__STRUCT_H_
