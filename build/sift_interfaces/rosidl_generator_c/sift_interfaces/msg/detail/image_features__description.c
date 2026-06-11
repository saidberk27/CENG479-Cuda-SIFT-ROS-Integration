// generated from rosidl_generator_c/resource/idl__description.c.em
// with input from sift_interfaces:msg/ImageFeatures.idl
// generated code does not contain a copyright notice

#include "sift_interfaces/msg/detail/image_features__functions.h"

ROSIDL_GENERATOR_C_PUBLIC_sift_interfaces
const rosidl_type_hash_t *
sift_interfaces__msg__ImageFeatures__get_type_hash(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_type_hash_t hash = {1, {
      0x19, 0x11, 0xd6, 0xf3, 0x1a, 0x38, 0xea, 0x42,
      0x24, 0x45, 0xcf, 0x2f, 0xdb, 0xff, 0xc5, 0xb3,
      0x2a, 0x14, 0x60, 0x98, 0x36, 0x60, 0x13, 0x58,
      0xe8, 0x58, 0xd3, 0x10, 0xd5, 0x16, 0x06, 0x68,
    }};
  return &hash;
}

#include <assert.h>
#include <string.h>

// Include directives for referenced types

// Hashes for external referenced types
#ifndef NDEBUG
#endif

static char sift_interfaces__msg__ImageFeatures__TYPE_NAME[] = "sift_interfaces/msg/ImageFeatures";

// Define type names, field names, and default values
static char sift_interfaces__msg__ImageFeatures__FIELD_NAME__image_name[] = "image_name";
static char sift_interfaces__msg__ImageFeatures__FIELD_NAME__height[] = "height";
static char sift_interfaces__msg__ImageFeatures__FIELD_NAME__width[] = "width";
static char sift_interfaces__msg__ImageFeatures__FIELD_NAME__keypoints_x[] = "keypoints_x";
static char sift_interfaces__msg__ImageFeatures__FIELD_NAME__keypoints_y[] = "keypoints_y";
static char sift_interfaces__msg__ImageFeatures__FIELD_NAME__keypoints_size[] = "keypoints_size";
static char sift_interfaces__msg__ImageFeatures__FIELD_NAME__keypoints_angle[] = "keypoints_angle";
static char sift_interfaces__msg__ImageFeatures__FIELD_NAME__descriptors[] = "descriptors";
static char sift_interfaces__msg__ImageFeatures__FIELD_NAME__num_keypoints[] = "num_keypoints";
static char sift_interfaces__msg__ImageFeatures__FIELD_NAME__publish_timestamp[] = "publish_timestamp";

static rosidl_runtime_c__type_description__Field sift_interfaces__msg__ImageFeatures__FIELDS[] = {
  {
    {sift_interfaces__msg__ImageFeatures__FIELD_NAME__image_name, 10, 10},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_STRING,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {sift_interfaces__msg__ImageFeatures__FIELD_NAME__height, 6, 6},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_INT32,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {sift_interfaces__msg__ImageFeatures__FIELD_NAME__width, 5, 5},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_INT32,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {sift_interfaces__msg__ImageFeatures__FIELD_NAME__keypoints_x, 11, 11},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_FLOAT_UNBOUNDED_SEQUENCE,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {sift_interfaces__msg__ImageFeatures__FIELD_NAME__keypoints_y, 11, 11},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_FLOAT_UNBOUNDED_SEQUENCE,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {sift_interfaces__msg__ImageFeatures__FIELD_NAME__keypoints_size, 14, 14},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_FLOAT_UNBOUNDED_SEQUENCE,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {sift_interfaces__msg__ImageFeatures__FIELD_NAME__keypoints_angle, 15, 15},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_FLOAT_UNBOUNDED_SEQUENCE,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {sift_interfaces__msg__ImageFeatures__FIELD_NAME__descriptors, 11, 11},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_FLOAT_UNBOUNDED_SEQUENCE,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {sift_interfaces__msg__ImageFeatures__FIELD_NAME__num_keypoints, 13, 13},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_INT32,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {sift_interfaces__msg__ImageFeatures__FIELD_NAME__publish_timestamp, 17, 17},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_DOUBLE,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
};

const rosidl_runtime_c__type_description__TypeDescription *
sift_interfaces__msg__ImageFeatures__get_type_description(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static bool constructed = false;
  static const rosidl_runtime_c__type_description__TypeDescription description = {
    {
      {sift_interfaces__msg__ImageFeatures__TYPE_NAME, 33, 33},
      {sift_interfaces__msg__ImageFeatures__FIELDS, 10, 10},
    },
    {NULL, 0, 0},
  };
  if (!constructed) {
    constructed = true;
  }
  return &description;
}

static char toplevel_type_raw_source[] =
  "# Image meta information\n"
  "string image_name\n"
  "int32 height\n"
  "int32 width\n"
  "\n"
  "# Keypoints (n keypoints)\n"
  "# Each keypoint has: x, y, size, angle\n"
  "float32[] keypoints_x\n"
  "float32[] keypoints_y  \n"
  "float32[] keypoints_size\n"
  "float32[] keypoints_angle\n"
  "\n"
  "# Descriptors flattened: n x 128 floats\n"
  "# Row-major order: descriptor_0[0..127], descriptor_1[0..127], ...\n"
  "float32[] descriptors\n"
  "\n"
  "# Number of keypoints\n"
  "int32 num_keypoints\n"
  "\n"
  "# Publish timestamp (seconds since epoch, set right before publishing)\n"
  "# Used by the subscriber to measure transport latency\n"
  "float64 publish_timestamp";

static char msg_encoding[] = "msg";

// Define all individual source functions

const rosidl_runtime_c__type_description__TypeSource *
sift_interfaces__msg__ImageFeatures__get_individual_type_description_source(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static const rosidl_runtime_c__type_description__TypeSource source = {
    {sift_interfaces__msg__ImageFeatures__TYPE_NAME, 33, 33},
    {msg_encoding, 3, 3},
    {toplevel_type_raw_source, 556, 556},
  };
  return &source;
}

const rosidl_runtime_c__type_description__TypeSource__Sequence *
sift_interfaces__msg__ImageFeatures__get_type_description_sources(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_runtime_c__type_description__TypeSource sources[1];
  static const rosidl_runtime_c__type_description__TypeSource__Sequence source_sequence = {sources, 1, 1};
  static bool constructed = false;
  if (!constructed) {
    sources[0] = *sift_interfaces__msg__ImageFeatures__get_individual_type_description_source(NULL),
    constructed = true;
  }
  return &source_sequence;
}
