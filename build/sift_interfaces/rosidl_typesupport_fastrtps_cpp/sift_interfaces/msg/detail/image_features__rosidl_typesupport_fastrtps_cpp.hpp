// generated from rosidl_typesupport_fastrtps_cpp/resource/idl__rosidl_typesupport_fastrtps_cpp.hpp.em
// with input from sift_interfaces:msg/ImageFeatures.idl
// generated code does not contain a copyright notice

#ifndef SIFT_INTERFACES__MSG__DETAIL__IMAGE_FEATURES__ROSIDL_TYPESUPPORT_FASTRTPS_CPP_HPP_
#define SIFT_INTERFACES__MSG__DETAIL__IMAGE_FEATURES__ROSIDL_TYPESUPPORT_FASTRTPS_CPP_HPP_

#include <cstddef>
#include "rosidl_runtime_c/message_type_support_struct.h"
#include "rosidl_typesupport_interface/macros.h"
#include "sift_interfaces/msg/rosidl_typesupport_fastrtps_cpp__visibility_control.h"
#include "sift_interfaces/msg/detail/image_features__struct.hpp"

#ifndef _WIN32
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wunused-parameter"
# ifdef __clang__
#  pragma clang diagnostic ignored "-Wdeprecated-register"
#  pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
# endif
#endif
#ifndef _WIN32
# pragma GCC diagnostic pop
#endif

#include "fastcdr/Cdr.h"

namespace sift_interfaces
{

namespace msg
{

namespace typesupport_fastrtps_cpp
{

bool
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_sift_interfaces
cdr_serialize(
  const sift_interfaces::msg::ImageFeatures & ros_message,
  eprosima::fastcdr::Cdr & cdr);

bool
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_sift_interfaces
cdr_deserialize(
  eprosima::fastcdr::Cdr & cdr,
  sift_interfaces::msg::ImageFeatures & ros_message);

size_t
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_sift_interfaces
get_serialized_size(
  const sift_interfaces::msg::ImageFeatures & ros_message,
  size_t current_alignment);

size_t
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_sift_interfaces
max_serialized_size_ImageFeatures(
  bool & full_bounded,
  bool & is_plain,
  size_t current_alignment);

bool
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_sift_interfaces
cdr_serialize_key(
  const sift_interfaces::msg::ImageFeatures & ros_message,
  eprosima::fastcdr::Cdr &);

size_t
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_sift_interfaces
get_serialized_size_key(
  const sift_interfaces::msg::ImageFeatures & ros_message,
  size_t current_alignment);

size_t
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_sift_interfaces
max_serialized_size_key_ImageFeatures(
  bool & full_bounded,
  bool & is_plain,
  size_t current_alignment);

}  // namespace typesupport_fastrtps_cpp

}  // namespace msg

}  // namespace sift_interfaces

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_sift_interfaces
const rosidl_message_type_support_t *
  ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_cpp, sift_interfaces, msg, ImageFeatures)();

#ifdef __cplusplus
}
#endif

#endif  // SIFT_INTERFACES__MSG__DETAIL__IMAGE_FEATURES__ROSIDL_TYPESUPPORT_FASTRTPS_CPP_HPP_
