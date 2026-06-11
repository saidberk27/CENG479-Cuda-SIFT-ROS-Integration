// generated from rosidl_typesupport_introspection_c/resource/idl__type_support.c.em
// with input from sift_interfaces:msg/ImageFeatures.idl
// generated code does not contain a copyright notice

#include <stddef.h>
#include "sift_interfaces/msg/detail/image_features__rosidl_typesupport_introspection_c.h"
#include "sift_interfaces/msg/rosidl_typesupport_introspection_c__visibility_control.h"
#include "rosidl_typesupport_introspection_c/field_types.h"
#include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/message_introspection.h"
#include "sift_interfaces/msg/detail/image_features__functions.h"
#include "sift_interfaces/msg/detail/image_features__struct.h"


// Include directives for member types
// Member `image_name`
#include "rosidl_runtime_c/string_functions.h"
// Member `keypoints_x`
// Member `keypoints_y`
// Member `keypoints_size`
// Member `keypoints_angle`
// Member `descriptors`
#include "rosidl_runtime_c/primitives_sequence_functions.h"

#ifdef __cplusplus
extern "C"
{
#endif

void sift_interfaces__msg__ImageFeatures__rosidl_typesupport_introspection_c__ImageFeatures_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  sift_interfaces__msg__ImageFeatures__init(message_memory);
}

void sift_interfaces__msg__ImageFeatures__rosidl_typesupport_introspection_c__ImageFeatures_fini_function(void * message_memory)
{
  sift_interfaces__msg__ImageFeatures__fini(message_memory);
}

size_t sift_interfaces__msg__ImageFeatures__rosidl_typesupport_introspection_c__size_function__ImageFeatures__keypoints_x(
  const void * untyped_member)
{
  const rosidl_runtime_c__float__Sequence * member =
    (const rosidl_runtime_c__float__Sequence *)(untyped_member);
  return member->size;
}

const void * sift_interfaces__msg__ImageFeatures__rosidl_typesupport_introspection_c__get_const_function__ImageFeatures__keypoints_x(
  const void * untyped_member, size_t index)
{
  const rosidl_runtime_c__float__Sequence * member =
    (const rosidl_runtime_c__float__Sequence *)(untyped_member);
  return &member->data[index];
}

void * sift_interfaces__msg__ImageFeatures__rosidl_typesupport_introspection_c__get_function__ImageFeatures__keypoints_x(
  void * untyped_member, size_t index)
{
  rosidl_runtime_c__float__Sequence * member =
    (rosidl_runtime_c__float__Sequence *)(untyped_member);
  return &member->data[index];
}

void sift_interfaces__msg__ImageFeatures__rosidl_typesupport_introspection_c__fetch_function__ImageFeatures__keypoints_x(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const float * item =
    ((const float *)
    sift_interfaces__msg__ImageFeatures__rosidl_typesupport_introspection_c__get_const_function__ImageFeatures__keypoints_x(untyped_member, index));
  float * value =
    (float *)(untyped_value);
  *value = *item;
}

void sift_interfaces__msg__ImageFeatures__rosidl_typesupport_introspection_c__assign_function__ImageFeatures__keypoints_x(
  void * untyped_member, size_t index, const void * untyped_value)
{
  float * item =
    ((float *)
    sift_interfaces__msg__ImageFeatures__rosidl_typesupport_introspection_c__get_function__ImageFeatures__keypoints_x(untyped_member, index));
  const float * value =
    (const float *)(untyped_value);
  *item = *value;
}

bool sift_interfaces__msg__ImageFeatures__rosidl_typesupport_introspection_c__resize_function__ImageFeatures__keypoints_x(
  void * untyped_member, size_t size)
{
  rosidl_runtime_c__float__Sequence * member =
    (rosidl_runtime_c__float__Sequence *)(untyped_member);
  rosidl_runtime_c__float__Sequence__fini(member);
  return rosidl_runtime_c__float__Sequence__init(member, size);
}

size_t sift_interfaces__msg__ImageFeatures__rosidl_typesupport_introspection_c__size_function__ImageFeatures__keypoints_y(
  const void * untyped_member)
{
  const rosidl_runtime_c__float__Sequence * member =
    (const rosidl_runtime_c__float__Sequence *)(untyped_member);
  return member->size;
}

const void * sift_interfaces__msg__ImageFeatures__rosidl_typesupport_introspection_c__get_const_function__ImageFeatures__keypoints_y(
  const void * untyped_member, size_t index)
{
  const rosidl_runtime_c__float__Sequence * member =
    (const rosidl_runtime_c__float__Sequence *)(untyped_member);
  return &member->data[index];
}

void * sift_interfaces__msg__ImageFeatures__rosidl_typesupport_introspection_c__get_function__ImageFeatures__keypoints_y(
  void * untyped_member, size_t index)
{
  rosidl_runtime_c__float__Sequence * member =
    (rosidl_runtime_c__float__Sequence *)(untyped_member);
  return &member->data[index];
}

void sift_interfaces__msg__ImageFeatures__rosidl_typesupport_introspection_c__fetch_function__ImageFeatures__keypoints_y(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const float * item =
    ((const float *)
    sift_interfaces__msg__ImageFeatures__rosidl_typesupport_introspection_c__get_const_function__ImageFeatures__keypoints_y(untyped_member, index));
  float * value =
    (float *)(untyped_value);
  *value = *item;
}

void sift_interfaces__msg__ImageFeatures__rosidl_typesupport_introspection_c__assign_function__ImageFeatures__keypoints_y(
  void * untyped_member, size_t index, const void * untyped_value)
{
  float * item =
    ((float *)
    sift_interfaces__msg__ImageFeatures__rosidl_typesupport_introspection_c__get_function__ImageFeatures__keypoints_y(untyped_member, index));
  const float * value =
    (const float *)(untyped_value);
  *item = *value;
}

bool sift_interfaces__msg__ImageFeatures__rosidl_typesupport_introspection_c__resize_function__ImageFeatures__keypoints_y(
  void * untyped_member, size_t size)
{
  rosidl_runtime_c__float__Sequence * member =
    (rosidl_runtime_c__float__Sequence *)(untyped_member);
  rosidl_runtime_c__float__Sequence__fini(member);
  return rosidl_runtime_c__float__Sequence__init(member, size);
}

size_t sift_interfaces__msg__ImageFeatures__rosidl_typesupport_introspection_c__size_function__ImageFeatures__keypoints_size(
  const void * untyped_member)
{
  const rosidl_runtime_c__float__Sequence * member =
    (const rosidl_runtime_c__float__Sequence *)(untyped_member);
  return member->size;
}

const void * sift_interfaces__msg__ImageFeatures__rosidl_typesupport_introspection_c__get_const_function__ImageFeatures__keypoints_size(
  const void * untyped_member, size_t index)
{
  const rosidl_runtime_c__float__Sequence * member =
    (const rosidl_runtime_c__float__Sequence *)(untyped_member);
  return &member->data[index];
}

void * sift_interfaces__msg__ImageFeatures__rosidl_typesupport_introspection_c__get_function__ImageFeatures__keypoints_size(
  void * untyped_member, size_t index)
{
  rosidl_runtime_c__float__Sequence * member =
    (rosidl_runtime_c__float__Sequence *)(untyped_member);
  return &member->data[index];
}

void sift_interfaces__msg__ImageFeatures__rosidl_typesupport_introspection_c__fetch_function__ImageFeatures__keypoints_size(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const float * item =
    ((const float *)
    sift_interfaces__msg__ImageFeatures__rosidl_typesupport_introspection_c__get_const_function__ImageFeatures__keypoints_size(untyped_member, index));
  float * value =
    (float *)(untyped_value);
  *value = *item;
}

void sift_interfaces__msg__ImageFeatures__rosidl_typesupport_introspection_c__assign_function__ImageFeatures__keypoints_size(
  void * untyped_member, size_t index, const void * untyped_value)
{
  float * item =
    ((float *)
    sift_interfaces__msg__ImageFeatures__rosidl_typesupport_introspection_c__get_function__ImageFeatures__keypoints_size(untyped_member, index));
  const float * value =
    (const float *)(untyped_value);
  *item = *value;
}

bool sift_interfaces__msg__ImageFeatures__rosidl_typesupport_introspection_c__resize_function__ImageFeatures__keypoints_size(
  void * untyped_member, size_t size)
{
  rosidl_runtime_c__float__Sequence * member =
    (rosidl_runtime_c__float__Sequence *)(untyped_member);
  rosidl_runtime_c__float__Sequence__fini(member);
  return rosidl_runtime_c__float__Sequence__init(member, size);
}

size_t sift_interfaces__msg__ImageFeatures__rosidl_typesupport_introspection_c__size_function__ImageFeatures__keypoints_angle(
  const void * untyped_member)
{
  const rosidl_runtime_c__float__Sequence * member =
    (const rosidl_runtime_c__float__Sequence *)(untyped_member);
  return member->size;
}

const void * sift_interfaces__msg__ImageFeatures__rosidl_typesupport_introspection_c__get_const_function__ImageFeatures__keypoints_angle(
  const void * untyped_member, size_t index)
{
  const rosidl_runtime_c__float__Sequence * member =
    (const rosidl_runtime_c__float__Sequence *)(untyped_member);
  return &member->data[index];
}

void * sift_interfaces__msg__ImageFeatures__rosidl_typesupport_introspection_c__get_function__ImageFeatures__keypoints_angle(
  void * untyped_member, size_t index)
{
  rosidl_runtime_c__float__Sequence * member =
    (rosidl_runtime_c__float__Sequence *)(untyped_member);
  return &member->data[index];
}

void sift_interfaces__msg__ImageFeatures__rosidl_typesupport_introspection_c__fetch_function__ImageFeatures__keypoints_angle(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const float * item =
    ((const float *)
    sift_interfaces__msg__ImageFeatures__rosidl_typesupport_introspection_c__get_const_function__ImageFeatures__keypoints_angle(untyped_member, index));
  float * value =
    (float *)(untyped_value);
  *value = *item;
}

void sift_interfaces__msg__ImageFeatures__rosidl_typesupport_introspection_c__assign_function__ImageFeatures__keypoints_angle(
  void * untyped_member, size_t index, const void * untyped_value)
{
  float * item =
    ((float *)
    sift_interfaces__msg__ImageFeatures__rosidl_typesupport_introspection_c__get_function__ImageFeatures__keypoints_angle(untyped_member, index));
  const float * value =
    (const float *)(untyped_value);
  *item = *value;
}

bool sift_interfaces__msg__ImageFeatures__rosidl_typesupport_introspection_c__resize_function__ImageFeatures__keypoints_angle(
  void * untyped_member, size_t size)
{
  rosidl_runtime_c__float__Sequence * member =
    (rosidl_runtime_c__float__Sequence *)(untyped_member);
  rosidl_runtime_c__float__Sequence__fini(member);
  return rosidl_runtime_c__float__Sequence__init(member, size);
}

size_t sift_interfaces__msg__ImageFeatures__rosidl_typesupport_introspection_c__size_function__ImageFeatures__descriptors(
  const void * untyped_member)
{
  const rosidl_runtime_c__float__Sequence * member =
    (const rosidl_runtime_c__float__Sequence *)(untyped_member);
  return member->size;
}

const void * sift_interfaces__msg__ImageFeatures__rosidl_typesupport_introspection_c__get_const_function__ImageFeatures__descriptors(
  const void * untyped_member, size_t index)
{
  const rosidl_runtime_c__float__Sequence * member =
    (const rosidl_runtime_c__float__Sequence *)(untyped_member);
  return &member->data[index];
}

void * sift_interfaces__msg__ImageFeatures__rosidl_typesupport_introspection_c__get_function__ImageFeatures__descriptors(
  void * untyped_member, size_t index)
{
  rosidl_runtime_c__float__Sequence * member =
    (rosidl_runtime_c__float__Sequence *)(untyped_member);
  return &member->data[index];
}

void sift_interfaces__msg__ImageFeatures__rosidl_typesupport_introspection_c__fetch_function__ImageFeatures__descriptors(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const float * item =
    ((const float *)
    sift_interfaces__msg__ImageFeatures__rosidl_typesupport_introspection_c__get_const_function__ImageFeatures__descriptors(untyped_member, index));
  float * value =
    (float *)(untyped_value);
  *value = *item;
}

void sift_interfaces__msg__ImageFeatures__rosidl_typesupport_introspection_c__assign_function__ImageFeatures__descriptors(
  void * untyped_member, size_t index, const void * untyped_value)
{
  float * item =
    ((float *)
    sift_interfaces__msg__ImageFeatures__rosidl_typesupport_introspection_c__get_function__ImageFeatures__descriptors(untyped_member, index));
  const float * value =
    (const float *)(untyped_value);
  *item = *value;
}

bool sift_interfaces__msg__ImageFeatures__rosidl_typesupport_introspection_c__resize_function__ImageFeatures__descriptors(
  void * untyped_member, size_t size)
{
  rosidl_runtime_c__float__Sequence * member =
    (rosidl_runtime_c__float__Sequence *)(untyped_member);
  rosidl_runtime_c__float__Sequence__fini(member);
  return rosidl_runtime_c__float__Sequence__init(member, size);
}

static rosidl_typesupport_introspection_c__MessageMember sift_interfaces__msg__ImageFeatures__rosidl_typesupport_introspection_c__ImageFeatures_message_member_array[10] = {
  {
    "image_name",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_STRING,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(sift_interfaces__msg__ImageFeatures, image_name),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "height",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_INT32,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(sift_interfaces__msg__ImageFeatures, height),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "width",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_INT32,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(sift_interfaces__msg__ImageFeatures, width),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "keypoints_x",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_FLOAT,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(sift_interfaces__msg__ImageFeatures, keypoints_x),  // bytes offset in struct
    NULL,  // default value
    sift_interfaces__msg__ImageFeatures__rosidl_typesupport_introspection_c__size_function__ImageFeatures__keypoints_x,  // size() function pointer
    sift_interfaces__msg__ImageFeatures__rosidl_typesupport_introspection_c__get_const_function__ImageFeatures__keypoints_x,  // get_const(index) function pointer
    sift_interfaces__msg__ImageFeatures__rosidl_typesupport_introspection_c__get_function__ImageFeatures__keypoints_x,  // get(index) function pointer
    sift_interfaces__msg__ImageFeatures__rosidl_typesupport_introspection_c__fetch_function__ImageFeatures__keypoints_x,  // fetch(index, &value) function pointer
    sift_interfaces__msg__ImageFeatures__rosidl_typesupport_introspection_c__assign_function__ImageFeatures__keypoints_x,  // assign(index, value) function pointer
    sift_interfaces__msg__ImageFeatures__rosidl_typesupport_introspection_c__resize_function__ImageFeatures__keypoints_x  // resize(index) function pointer
  },
  {
    "keypoints_y",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_FLOAT,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(sift_interfaces__msg__ImageFeatures, keypoints_y),  // bytes offset in struct
    NULL,  // default value
    sift_interfaces__msg__ImageFeatures__rosidl_typesupport_introspection_c__size_function__ImageFeatures__keypoints_y,  // size() function pointer
    sift_interfaces__msg__ImageFeatures__rosidl_typesupport_introspection_c__get_const_function__ImageFeatures__keypoints_y,  // get_const(index) function pointer
    sift_interfaces__msg__ImageFeatures__rosidl_typesupport_introspection_c__get_function__ImageFeatures__keypoints_y,  // get(index) function pointer
    sift_interfaces__msg__ImageFeatures__rosidl_typesupport_introspection_c__fetch_function__ImageFeatures__keypoints_y,  // fetch(index, &value) function pointer
    sift_interfaces__msg__ImageFeatures__rosidl_typesupport_introspection_c__assign_function__ImageFeatures__keypoints_y,  // assign(index, value) function pointer
    sift_interfaces__msg__ImageFeatures__rosidl_typesupport_introspection_c__resize_function__ImageFeatures__keypoints_y  // resize(index) function pointer
  },
  {
    "keypoints_size",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_FLOAT,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(sift_interfaces__msg__ImageFeatures, keypoints_size),  // bytes offset in struct
    NULL,  // default value
    sift_interfaces__msg__ImageFeatures__rosidl_typesupport_introspection_c__size_function__ImageFeatures__keypoints_size,  // size() function pointer
    sift_interfaces__msg__ImageFeatures__rosidl_typesupport_introspection_c__get_const_function__ImageFeatures__keypoints_size,  // get_const(index) function pointer
    sift_interfaces__msg__ImageFeatures__rosidl_typesupport_introspection_c__get_function__ImageFeatures__keypoints_size,  // get(index) function pointer
    sift_interfaces__msg__ImageFeatures__rosidl_typesupport_introspection_c__fetch_function__ImageFeatures__keypoints_size,  // fetch(index, &value) function pointer
    sift_interfaces__msg__ImageFeatures__rosidl_typesupport_introspection_c__assign_function__ImageFeatures__keypoints_size,  // assign(index, value) function pointer
    sift_interfaces__msg__ImageFeatures__rosidl_typesupport_introspection_c__resize_function__ImageFeatures__keypoints_size  // resize(index) function pointer
  },
  {
    "keypoints_angle",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_FLOAT,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(sift_interfaces__msg__ImageFeatures, keypoints_angle),  // bytes offset in struct
    NULL,  // default value
    sift_interfaces__msg__ImageFeatures__rosidl_typesupport_introspection_c__size_function__ImageFeatures__keypoints_angle,  // size() function pointer
    sift_interfaces__msg__ImageFeatures__rosidl_typesupport_introspection_c__get_const_function__ImageFeatures__keypoints_angle,  // get_const(index) function pointer
    sift_interfaces__msg__ImageFeatures__rosidl_typesupport_introspection_c__get_function__ImageFeatures__keypoints_angle,  // get(index) function pointer
    sift_interfaces__msg__ImageFeatures__rosidl_typesupport_introspection_c__fetch_function__ImageFeatures__keypoints_angle,  // fetch(index, &value) function pointer
    sift_interfaces__msg__ImageFeatures__rosidl_typesupport_introspection_c__assign_function__ImageFeatures__keypoints_angle,  // assign(index, value) function pointer
    sift_interfaces__msg__ImageFeatures__rosidl_typesupport_introspection_c__resize_function__ImageFeatures__keypoints_angle  // resize(index) function pointer
  },
  {
    "descriptors",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_FLOAT,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(sift_interfaces__msg__ImageFeatures, descriptors),  // bytes offset in struct
    NULL,  // default value
    sift_interfaces__msg__ImageFeatures__rosidl_typesupport_introspection_c__size_function__ImageFeatures__descriptors,  // size() function pointer
    sift_interfaces__msg__ImageFeatures__rosidl_typesupport_introspection_c__get_const_function__ImageFeatures__descriptors,  // get_const(index) function pointer
    sift_interfaces__msg__ImageFeatures__rosidl_typesupport_introspection_c__get_function__ImageFeatures__descriptors,  // get(index) function pointer
    sift_interfaces__msg__ImageFeatures__rosidl_typesupport_introspection_c__fetch_function__ImageFeatures__descriptors,  // fetch(index, &value) function pointer
    sift_interfaces__msg__ImageFeatures__rosidl_typesupport_introspection_c__assign_function__ImageFeatures__descriptors,  // assign(index, value) function pointer
    sift_interfaces__msg__ImageFeatures__rosidl_typesupport_introspection_c__resize_function__ImageFeatures__descriptors  // resize(index) function pointer
  },
  {
    "num_keypoints",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_INT32,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(sift_interfaces__msg__ImageFeatures, num_keypoints),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "publish_timestamp",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_DOUBLE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(sift_interfaces__msg__ImageFeatures, publish_timestamp),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers sift_interfaces__msg__ImageFeatures__rosidl_typesupport_introspection_c__ImageFeatures_message_members = {
  "sift_interfaces__msg",  // message namespace
  "ImageFeatures",  // message name
  10,  // number of fields
  sizeof(sift_interfaces__msg__ImageFeatures),
  false,  // has_any_key_member_
  sift_interfaces__msg__ImageFeatures__rosidl_typesupport_introspection_c__ImageFeatures_message_member_array,  // message members
  sift_interfaces__msg__ImageFeatures__rosidl_typesupport_introspection_c__ImageFeatures_init_function,  // function to initialize message memory (memory has to be allocated)
  sift_interfaces__msg__ImageFeatures__rosidl_typesupport_introspection_c__ImageFeatures_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t sift_interfaces__msg__ImageFeatures__rosidl_typesupport_introspection_c__ImageFeatures_message_type_support_handle = {
  0,
  &sift_interfaces__msg__ImageFeatures__rosidl_typesupport_introspection_c__ImageFeatures_message_members,
  get_message_typesupport_handle_function,
  &sift_interfaces__msg__ImageFeatures__get_type_hash,
  &sift_interfaces__msg__ImageFeatures__get_type_description,
  &sift_interfaces__msg__ImageFeatures__get_type_description_sources,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_sift_interfaces
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, sift_interfaces, msg, ImageFeatures)() {
  if (!sift_interfaces__msg__ImageFeatures__rosidl_typesupport_introspection_c__ImageFeatures_message_type_support_handle.typesupport_identifier) {
    sift_interfaces__msg__ImageFeatures__rosidl_typesupport_introspection_c__ImageFeatures_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &sift_interfaces__msg__ImageFeatures__rosidl_typesupport_introspection_c__ImageFeatures_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif
