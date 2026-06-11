// generated from rosidl_typesupport_introspection_cpp/resource/idl__type_support.cpp.em
// with input from sift_interfaces:msg/ImageFeatures.idl
// generated code does not contain a copyright notice

#include "array"
#include "cstddef"
#include "string"
#include "vector"
#include "rosidl_runtime_c/message_type_support_struct.h"
#include "rosidl_typesupport_cpp/message_type_support.hpp"
#include "rosidl_typesupport_interface/macros.h"
#include "sift_interfaces/msg/detail/image_features__functions.h"
#include "sift_interfaces/msg/detail/image_features__struct.hpp"
#include "rosidl_typesupport_introspection_cpp/field_types.hpp"
#include "rosidl_typesupport_introspection_cpp/identifier.hpp"
#include "rosidl_typesupport_introspection_cpp/message_introspection.hpp"
#include "rosidl_typesupport_introspection_cpp/message_type_support_decl.hpp"
#include "rosidl_typesupport_introspection_cpp/visibility_control.h"

namespace sift_interfaces
{

namespace msg
{

namespace rosidl_typesupport_introspection_cpp
{

void ImageFeatures_init_function(
  void * message_memory, rosidl_runtime_cpp::MessageInitialization _init)
{
  new (message_memory) sift_interfaces::msg::ImageFeatures(_init);
}

void ImageFeatures_fini_function(void * message_memory)
{
  auto typed_message = static_cast<sift_interfaces::msg::ImageFeatures *>(message_memory);
  typed_message->~ImageFeatures();
}

size_t size_function__ImageFeatures__keypoints_x(const void * untyped_member)
{
  const auto * member = reinterpret_cast<const std::vector<float> *>(untyped_member);
  return member->size();
}

const void * get_const_function__ImageFeatures__keypoints_x(const void * untyped_member, size_t index)
{
  const auto & member =
    *reinterpret_cast<const std::vector<float> *>(untyped_member);
  return &member[index];
}

void * get_function__ImageFeatures__keypoints_x(void * untyped_member, size_t index)
{
  auto & member =
    *reinterpret_cast<std::vector<float> *>(untyped_member);
  return &member[index];
}

void fetch_function__ImageFeatures__keypoints_x(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const auto & item = *reinterpret_cast<const float *>(
    get_const_function__ImageFeatures__keypoints_x(untyped_member, index));
  auto & value = *reinterpret_cast<float *>(untyped_value);
  value = item;
}

void assign_function__ImageFeatures__keypoints_x(
  void * untyped_member, size_t index, const void * untyped_value)
{
  auto & item = *reinterpret_cast<float *>(
    get_function__ImageFeatures__keypoints_x(untyped_member, index));
  const auto & value = *reinterpret_cast<const float *>(untyped_value);
  item = value;
}

void resize_function__ImageFeatures__keypoints_x(void * untyped_member, size_t size)
{
  auto * member =
    reinterpret_cast<std::vector<float> *>(untyped_member);
  member->resize(size);
}

size_t size_function__ImageFeatures__keypoints_y(const void * untyped_member)
{
  const auto * member = reinterpret_cast<const std::vector<float> *>(untyped_member);
  return member->size();
}

const void * get_const_function__ImageFeatures__keypoints_y(const void * untyped_member, size_t index)
{
  const auto & member =
    *reinterpret_cast<const std::vector<float> *>(untyped_member);
  return &member[index];
}

void * get_function__ImageFeatures__keypoints_y(void * untyped_member, size_t index)
{
  auto & member =
    *reinterpret_cast<std::vector<float> *>(untyped_member);
  return &member[index];
}

void fetch_function__ImageFeatures__keypoints_y(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const auto & item = *reinterpret_cast<const float *>(
    get_const_function__ImageFeatures__keypoints_y(untyped_member, index));
  auto & value = *reinterpret_cast<float *>(untyped_value);
  value = item;
}

void assign_function__ImageFeatures__keypoints_y(
  void * untyped_member, size_t index, const void * untyped_value)
{
  auto & item = *reinterpret_cast<float *>(
    get_function__ImageFeatures__keypoints_y(untyped_member, index));
  const auto & value = *reinterpret_cast<const float *>(untyped_value);
  item = value;
}

void resize_function__ImageFeatures__keypoints_y(void * untyped_member, size_t size)
{
  auto * member =
    reinterpret_cast<std::vector<float> *>(untyped_member);
  member->resize(size);
}

size_t size_function__ImageFeatures__keypoints_size(const void * untyped_member)
{
  const auto * member = reinterpret_cast<const std::vector<float> *>(untyped_member);
  return member->size();
}

const void * get_const_function__ImageFeatures__keypoints_size(const void * untyped_member, size_t index)
{
  const auto & member =
    *reinterpret_cast<const std::vector<float> *>(untyped_member);
  return &member[index];
}

void * get_function__ImageFeatures__keypoints_size(void * untyped_member, size_t index)
{
  auto & member =
    *reinterpret_cast<std::vector<float> *>(untyped_member);
  return &member[index];
}

void fetch_function__ImageFeatures__keypoints_size(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const auto & item = *reinterpret_cast<const float *>(
    get_const_function__ImageFeatures__keypoints_size(untyped_member, index));
  auto & value = *reinterpret_cast<float *>(untyped_value);
  value = item;
}

void assign_function__ImageFeatures__keypoints_size(
  void * untyped_member, size_t index, const void * untyped_value)
{
  auto & item = *reinterpret_cast<float *>(
    get_function__ImageFeatures__keypoints_size(untyped_member, index));
  const auto & value = *reinterpret_cast<const float *>(untyped_value);
  item = value;
}

void resize_function__ImageFeatures__keypoints_size(void * untyped_member, size_t size)
{
  auto * member =
    reinterpret_cast<std::vector<float> *>(untyped_member);
  member->resize(size);
}

size_t size_function__ImageFeatures__keypoints_angle(const void * untyped_member)
{
  const auto * member = reinterpret_cast<const std::vector<float> *>(untyped_member);
  return member->size();
}

const void * get_const_function__ImageFeatures__keypoints_angle(const void * untyped_member, size_t index)
{
  const auto & member =
    *reinterpret_cast<const std::vector<float> *>(untyped_member);
  return &member[index];
}

void * get_function__ImageFeatures__keypoints_angle(void * untyped_member, size_t index)
{
  auto & member =
    *reinterpret_cast<std::vector<float> *>(untyped_member);
  return &member[index];
}

void fetch_function__ImageFeatures__keypoints_angle(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const auto & item = *reinterpret_cast<const float *>(
    get_const_function__ImageFeatures__keypoints_angle(untyped_member, index));
  auto & value = *reinterpret_cast<float *>(untyped_value);
  value = item;
}

void assign_function__ImageFeatures__keypoints_angle(
  void * untyped_member, size_t index, const void * untyped_value)
{
  auto & item = *reinterpret_cast<float *>(
    get_function__ImageFeatures__keypoints_angle(untyped_member, index));
  const auto & value = *reinterpret_cast<const float *>(untyped_value);
  item = value;
}

void resize_function__ImageFeatures__keypoints_angle(void * untyped_member, size_t size)
{
  auto * member =
    reinterpret_cast<std::vector<float> *>(untyped_member);
  member->resize(size);
}

size_t size_function__ImageFeatures__descriptors(const void * untyped_member)
{
  const auto * member = reinterpret_cast<const std::vector<float> *>(untyped_member);
  return member->size();
}

const void * get_const_function__ImageFeatures__descriptors(const void * untyped_member, size_t index)
{
  const auto & member =
    *reinterpret_cast<const std::vector<float> *>(untyped_member);
  return &member[index];
}

void * get_function__ImageFeatures__descriptors(void * untyped_member, size_t index)
{
  auto & member =
    *reinterpret_cast<std::vector<float> *>(untyped_member);
  return &member[index];
}

void fetch_function__ImageFeatures__descriptors(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const auto & item = *reinterpret_cast<const float *>(
    get_const_function__ImageFeatures__descriptors(untyped_member, index));
  auto & value = *reinterpret_cast<float *>(untyped_value);
  value = item;
}

void assign_function__ImageFeatures__descriptors(
  void * untyped_member, size_t index, const void * untyped_value)
{
  auto & item = *reinterpret_cast<float *>(
    get_function__ImageFeatures__descriptors(untyped_member, index));
  const auto & value = *reinterpret_cast<const float *>(untyped_value);
  item = value;
}

void resize_function__ImageFeatures__descriptors(void * untyped_member, size_t size)
{
  auto * member =
    reinterpret_cast<std::vector<float> *>(untyped_member);
  member->resize(size);
}

static const ::rosidl_typesupport_introspection_cpp::MessageMember ImageFeatures_message_member_array[10] = {
  {
    "image_name",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_STRING,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(sift_interfaces::msg::ImageFeatures, image_name),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "height",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_INT32,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(sift_interfaces::msg::ImageFeatures, height),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "width",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_INT32,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(sift_interfaces::msg::ImageFeatures, width),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "keypoints_x",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_FLOAT,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is key
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(sift_interfaces::msg::ImageFeatures, keypoints_x),  // bytes offset in struct
    nullptr,  // default value
    size_function__ImageFeatures__keypoints_x,  // size() function pointer
    get_const_function__ImageFeatures__keypoints_x,  // get_const(index) function pointer
    get_function__ImageFeatures__keypoints_x,  // get(index) function pointer
    fetch_function__ImageFeatures__keypoints_x,  // fetch(index, &value) function pointer
    assign_function__ImageFeatures__keypoints_x,  // assign(index, value) function pointer
    resize_function__ImageFeatures__keypoints_x  // resize(index) function pointer
  },
  {
    "keypoints_y",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_FLOAT,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is key
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(sift_interfaces::msg::ImageFeatures, keypoints_y),  // bytes offset in struct
    nullptr,  // default value
    size_function__ImageFeatures__keypoints_y,  // size() function pointer
    get_const_function__ImageFeatures__keypoints_y,  // get_const(index) function pointer
    get_function__ImageFeatures__keypoints_y,  // get(index) function pointer
    fetch_function__ImageFeatures__keypoints_y,  // fetch(index, &value) function pointer
    assign_function__ImageFeatures__keypoints_y,  // assign(index, value) function pointer
    resize_function__ImageFeatures__keypoints_y  // resize(index) function pointer
  },
  {
    "keypoints_size",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_FLOAT,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is key
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(sift_interfaces::msg::ImageFeatures, keypoints_size),  // bytes offset in struct
    nullptr,  // default value
    size_function__ImageFeatures__keypoints_size,  // size() function pointer
    get_const_function__ImageFeatures__keypoints_size,  // get_const(index) function pointer
    get_function__ImageFeatures__keypoints_size,  // get(index) function pointer
    fetch_function__ImageFeatures__keypoints_size,  // fetch(index, &value) function pointer
    assign_function__ImageFeatures__keypoints_size,  // assign(index, value) function pointer
    resize_function__ImageFeatures__keypoints_size  // resize(index) function pointer
  },
  {
    "keypoints_angle",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_FLOAT,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is key
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(sift_interfaces::msg::ImageFeatures, keypoints_angle),  // bytes offset in struct
    nullptr,  // default value
    size_function__ImageFeatures__keypoints_angle,  // size() function pointer
    get_const_function__ImageFeatures__keypoints_angle,  // get_const(index) function pointer
    get_function__ImageFeatures__keypoints_angle,  // get(index) function pointer
    fetch_function__ImageFeatures__keypoints_angle,  // fetch(index, &value) function pointer
    assign_function__ImageFeatures__keypoints_angle,  // assign(index, value) function pointer
    resize_function__ImageFeatures__keypoints_angle  // resize(index) function pointer
  },
  {
    "descriptors",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_FLOAT,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is key
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(sift_interfaces::msg::ImageFeatures, descriptors),  // bytes offset in struct
    nullptr,  // default value
    size_function__ImageFeatures__descriptors,  // size() function pointer
    get_const_function__ImageFeatures__descriptors,  // get_const(index) function pointer
    get_function__ImageFeatures__descriptors,  // get(index) function pointer
    fetch_function__ImageFeatures__descriptors,  // fetch(index, &value) function pointer
    assign_function__ImageFeatures__descriptors,  // assign(index, value) function pointer
    resize_function__ImageFeatures__descriptors  // resize(index) function pointer
  },
  {
    "num_keypoints",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_INT32,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(sift_interfaces::msg::ImageFeatures, num_keypoints),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "publish_timestamp",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_DOUBLE,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(sift_interfaces::msg::ImageFeatures, publish_timestamp),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  }
};

static const ::rosidl_typesupport_introspection_cpp::MessageMembers ImageFeatures_message_members = {
  "sift_interfaces::msg",  // message namespace
  "ImageFeatures",  // message name
  10,  // number of fields
  sizeof(sift_interfaces::msg::ImageFeatures),
  false,  // has_any_key_member_
  ImageFeatures_message_member_array,  // message members
  ImageFeatures_init_function,  // function to initialize message memory (memory has to be allocated)
  ImageFeatures_fini_function  // function to terminate message instance (will not free memory)
};

static const rosidl_message_type_support_t ImageFeatures_message_type_support_handle = {
  ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  &ImageFeatures_message_members,
  get_message_typesupport_handle_function,
  &sift_interfaces__msg__ImageFeatures__get_type_hash,
  &sift_interfaces__msg__ImageFeatures__get_type_description,
  &sift_interfaces__msg__ImageFeatures__get_type_description_sources,
};

}  // namespace rosidl_typesupport_introspection_cpp

}  // namespace msg

}  // namespace sift_interfaces


namespace rosidl_typesupport_introspection_cpp
{

template<>
ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
get_message_type_support_handle<sift_interfaces::msg::ImageFeatures>()
{
  return &::sift_interfaces::msg::rosidl_typesupport_introspection_cpp::ImageFeatures_message_type_support_handle;
}

}  // namespace rosidl_typesupport_introspection_cpp

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, sift_interfaces, msg, ImageFeatures)() {
  return &::sift_interfaces::msg::rosidl_typesupport_introspection_cpp::ImageFeatures_message_type_support_handle;
}

#ifdef __cplusplus
}
#endif
