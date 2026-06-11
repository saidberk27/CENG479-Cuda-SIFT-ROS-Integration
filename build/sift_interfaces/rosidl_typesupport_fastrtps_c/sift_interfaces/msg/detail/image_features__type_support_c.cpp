// generated from rosidl_typesupport_fastrtps_c/resource/idl__type_support_c.cpp.em
// with input from sift_interfaces:msg/ImageFeatures.idl
// generated code does not contain a copyright notice
#include "sift_interfaces/msg/detail/image_features__rosidl_typesupport_fastrtps_c.h"


#include <cassert>
#include <cstddef>
#include <limits>
#include <string>
#include "rosidl_typesupport_fastrtps_c/identifier.h"
#include "rosidl_typesupport_fastrtps_c/serialization_helpers.hpp"
#include "rosidl_typesupport_fastrtps_c/wstring_conversion.hpp"
#include "rosidl_typesupport_fastrtps_cpp/message_type_support.h"
#include "sift_interfaces/msg/rosidl_typesupport_fastrtps_c__visibility_control.h"
#include "sift_interfaces/msg/detail/image_features__struct.h"
#include "sift_interfaces/msg/detail/image_features__functions.h"
#include "fastcdr/Cdr.h"

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

// includes and forward declarations of message dependencies and their conversion functions

#if defined(__cplusplus)
extern "C"
{
#endif

#include "rosidl_runtime_c/primitives_sequence.h"  // descriptors, keypoints_angle, keypoints_size, keypoints_x, keypoints_y
#include "rosidl_runtime_c/primitives_sequence_functions.h"  // descriptors, keypoints_angle, keypoints_size, keypoints_x, keypoints_y
#include "rosidl_runtime_c/string.h"  // image_name
#include "rosidl_runtime_c/string_functions.h"  // image_name

// forward declare type support functions


using _ImageFeatures__ros_msg_type = sift_interfaces__msg__ImageFeatures;


ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_sift_interfaces
bool cdr_serialize_sift_interfaces__msg__ImageFeatures(
  const sift_interfaces__msg__ImageFeatures * ros_message,
  eprosima::fastcdr::Cdr & cdr)
{
  // Field name: image_name
  {
    const rosidl_runtime_c__String * str = &ros_message->image_name;
    if (str->capacity == 0 || str->capacity <= str->size) {
      fprintf(stderr, "string capacity not greater than size\n");
      return false;
    }
    if (str->data[str->size] != '\0') {
      fprintf(stderr, "string not null-terminated\n");
      return false;
    }
    cdr << str->data;
  }

  // Field name: height
  {
    cdr << ros_message->height;
  }

  // Field name: width
  {
    cdr << ros_message->width;
  }

  // Field name: keypoints_x
  {
    size_t size = ros_message->keypoints_x.size;
    auto array_ptr = ros_message->keypoints_x.data;
    cdr << static_cast<uint32_t>(size);
    cdr.serialize_array(array_ptr, size);
  }

  // Field name: keypoints_y
  {
    size_t size = ros_message->keypoints_y.size;
    auto array_ptr = ros_message->keypoints_y.data;
    cdr << static_cast<uint32_t>(size);
    cdr.serialize_array(array_ptr, size);
  }

  // Field name: keypoints_size
  {
    size_t size = ros_message->keypoints_size.size;
    auto array_ptr = ros_message->keypoints_size.data;
    cdr << static_cast<uint32_t>(size);
    cdr.serialize_array(array_ptr, size);
  }

  // Field name: keypoints_angle
  {
    size_t size = ros_message->keypoints_angle.size;
    auto array_ptr = ros_message->keypoints_angle.data;
    cdr << static_cast<uint32_t>(size);
    cdr.serialize_array(array_ptr, size);
  }

  // Field name: descriptors
  {
    size_t size = ros_message->descriptors.size;
    auto array_ptr = ros_message->descriptors.data;
    cdr << static_cast<uint32_t>(size);
    cdr.serialize_array(array_ptr, size);
  }

  // Field name: num_keypoints
  {
    cdr << ros_message->num_keypoints;
  }

  // Field name: publish_timestamp
  {
    cdr << ros_message->publish_timestamp;
  }

  return true;
}

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_sift_interfaces
bool cdr_deserialize_sift_interfaces__msg__ImageFeatures(
  eprosima::fastcdr::Cdr & cdr,
  sift_interfaces__msg__ImageFeatures * ros_message)
{
  // Field name: image_name
  {
    std::string tmp;
    cdr >> tmp;
    if (!ros_message->image_name.data) {
      rosidl_runtime_c__String__init(&ros_message->image_name);
    }
    bool succeeded = rosidl_runtime_c__String__assign(
      &ros_message->image_name,
      tmp.c_str());
    if (!succeeded) {
      fprintf(stderr, "failed to assign string into field 'image_name'\n");
      return false;
    }
  }

  // Field name: height
  {
    cdr >> ros_message->height;
  }

  // Field name: width
  {
    cdr >> ros_message->width;
  }

  // Field name: keypoints_x
  {
    uint32_t cdrSize;
    cdr >> cdrSize;
    size_t size = static_cast<size_t>(cdrSize);

    // Check there are at least 'size' remaining bytes in the CDR stream before resizing
    auto old_state = cdr.get_state();
    bool correct_size = cdr.jump(size);
    cdr.set_state(old_state);
    if (!correct_size) {
      fprintf(stderr, "sequence size exceeds remaining buffer\n");
      return false;
    }

    if (ros_message->keypoints_x.data) {
      rosidl_runtime_c__float__Sequence__fini(&ros_message->keypoints_x);
    }
    if (!rosidl_runtime_c__float__Sequence__init(&ros_message->keypoints_x, size)) {
      fprintf(stderr, "failed to create array for field 'keypoints_x'");
      return false;
    }
    auto array_ptr = ros_message->keypoints_x.data;
    cdr.deserialize_array(array_ptr, size);
  }

  // Field name: keypoints_y
  {
    uint32_t cdrSize;
    cdr >> cdrSize;
    size_t size = static_cast<size_t>(cdrSize);

    // Check there are at least 'size' remaining bytes in the CDR stream before resizing
    auto old_state = cdr.get_state();
    bool correct_size = cdr.jump(size);
    cdr.set_state(old_state);
    if (!correct_size) {
      fprintf(stderr, "sequence size exceeds remaining buffer\n");
      return false;
    }

    if (ros_message->keypoints_y.data) {
      rosidl_runtime_c__float__Sequence__fini(&ros_message->keypoints_y);
    }
    if (!rosidl_runtime_c__float__Sequence__init(&ros_message->keypoints_y, size)) {
      fprintf(stderr, "failed to create array for field 'keypoints_y'");
      return false;
    }
    auto array_ptr = ros_message->keypoints_y.data;
    cdr.deserialize_array(array_ptr, size);
  }

  // Field name: keypoints_size
  {
    uint32_t cdrSize;
    cdr >> cdrSize;
    size_t size = static_cast<size_t>(cdrSize);

    // Check there are at least 'size' remaining bytes in the CDR stream before resizing
    auto old_state = cdr.get_state();
    bool correct_size = cdr.jump(size);
    cdr.set_state(old_state);
    if (!correct_size) {
      fprintf(stderr, "sequence size exceeds remaining buffer\n");
      return false;
    }

    if (ros_message->keypoints_size.data) {
      rosidl_runtime_c__float__Sequence__fini(&ros_message->keypoints_size);
    }
    if (!rosidl_runtime_c__float__Sequence__init(&ros_message->keypoints_size, size)) {
      fprintf(stderr, "failed to create array for field 'keypoints_size'");
      return false;
    }
    auto array_ptr = ros_message->keypoints_size.data;
    cdr.deserialize_array(array_ptr, size);
  }

  // Field name: keypoints_angle
  {
    uint32_t cdrSize;
    cdr >> cdrSize;
    size_t size = static_cast<size_t>(cdrSize);

    // Check there are at least 'size' remaining bytes in the CDR stream before resizing
    auto old_state = cdr.get_state();
    bool correct_size = cdr.jump(size);
    cdr.set_state(old_state);
    if (!correct_size) {
      fprintf(stderr, "sequence size exceeds remaining buffer\n");
      return false;
    }

    if (ros_message->keypoints_angle.data) {
      rosidl_runtime_c__float__Sequence__fini(&ros_message->keypoints_angle);
    }
    if (!rosidl_runtime_c__float__Sequence__init(&ros_message->keypoints_angle, size)) {
      fprintf(stderr, "failed to create array for field 'keypoints_angle'");
      return false;
    }
    auto array_ptr = ros_message->keypoints_angle.data;
    cdr.deserialize_array(array_ptr, size);
  }

  // Field name: descriptors
  {
    uint32_t cdrSize;
    cdr >> cdrSize;
    size_t size = static_cast<size_t>(cdrSize);

    // Check there are at least 'size' remaining bytes in the CDR stream before resizing
    auto old_state = cdr.get_state();
    bool correct_size = cdr.jump(size);
    cdr.set_state(old_state);
    if (!correct_size) {
      fprintf(stderr, "sequence size exceeds remaining buffer\n");
      return false;
    }

    if (ros_message->descriptors.data) {
      rosidl_runtime_c__float__Sequence__fini(&ros_message->descriptors);
    }
    if (!rosidl_runtime_c__float__Sequence__init(&ros_message->descriptors, size)) {
      fprintf(stderr, "failed to create array for field 'descriptors'");
      return false;
    }
    auto array_ptr = ros_message->descriptors.data;
    cdr.deserialize_array(array_ptr, size);
  }

  // Field name: num_keypoints
  {
    cdr >> ros_message->num_keypoints;
  }

  // Field name: publish_timestamp
  {
    cdr >> ros_message->publish_timestamp;
  }

  return true;
}  // NOLINT(readability/fn_size)


ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_sift_interfaces
size_t get_serialized_size_sift_interfaces__msg__ImageFeatures(
  const void * untyped_ros_message,
  size_t current_alignment)
{
  const _ImageFeatures__ros_msg_type * ros_message = static_cast<const _ImageFeatures__ros_msg_type *>(untyped_ros_message);
  (void)ros_message;
  size_t initial_alignment = current_alignment;

  const size_t padding = 4;
  const size_t wchar_size = 4;
  (void)padding;
  (void)wchar_size;

  // Field name: image_name
  current_alignment += padding +
    eprosima::fastcdr::Cdr::alignment(current_alignment, padding) +
    (ros_message->image_name.size + 1);

  // Field name: height
  {
    size_t item_size = sizeof(ros_message->height);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }

  // Field name: width
  {
    size_t item_size = sizeof(ros_message->width);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }

  // Field name: keypoints_x
  {
    size_t array_size = ros_message->keypoints_x.size;
    auto array_ptr = ros_message->keypoints_x.data;
    current_alignment += padding +
      eprosima::fastcdr::Cdr::alignment(current_alignment, padding);
    (void)array_ptr;
    size_t item_size = sizeof(array_ptr[0]);
    current_alignment += array_size * item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }

  // Field name: keypoints_y
  {
    size_t array_size = ros_message->keypoints_y.size;
    auto array_ptr = ros_message->keypoints_y.data;
    current_alignment += padding +
      eprosima::fastcdr::Cdr::alignment(current_alignment, padding);
    (void)array_ptr;
    size_t item_size = sizeof(array_ptr[0]);
    current_alignment += array_size * item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }

  // Field name: keypoints_size
  {
    size_t array_size = ros_message->keypoints_size.size;
    auto array_ptr = ros_message->keypoints_size.data;
    current_alignment += padding +
      eprosima::fastcdr::Cdr::alignment(current_alignment, padding);
    (void)array_ptr;
    size_t item_size = sizeof(array_ptr[0]);
    current_alignment += array_size * item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }

  // Field name: keypoints_angle
  {
    size_t array_size = ros_message->keypoints_angle.size;
    auto array_ptr = ros_message->keypoints_angle.data;
    current_alignment += padding +
      eprosima::fastcdr::Cdr::alignment(current_alignment, padding);
    (void)array_ptr;
    size_t item_size = sizeof(array_ptr[0]);
    current_alignment += array_size * item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }

  // Field name: descriptors
  {
    size_t array_size = ros_message->descriptors.size;
    auto array_ptr = ros_message->descriptors.data;
    current_alignment += padding +
      eprosima::fastcdr::Cdr::alignment(current_alignment, padding);
    (void)array_ptr;
    size_t item_size = sizeof(array_ptr[0]);
    current_alignment += array_size * item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }

  // Field name: num_keypoints
  {
    size_t item_size = sizeof(ros_message->num_keypoints);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }

  // Field name: publish_timestamp
  {
    size_t item_size = sizeof(ros_message->publish_timestamp);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }

  return current_alignment - initial_alignment;
}


ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_sift_interfaces
size_t max_serialized_size_sift_interfaces__msg__ImageFeatures(
  bool & full_bounded,
  bool & is_plain,
  size_t current_alignment)
{
  size_t initial_alignment = current_alignment;

  const size_t padding = 4;
  const size_t wchar_size = 4;
  size_t last_member_size = 0;
  (void)last_member_size;
  (void)padding;
  (void)wchar_size;

  full_bounded = true;
  is_plain = true;

  // Field name: image_name
  {
    size_t array_size = 1;
    full_bounded = false;
    is_plain = false;
    for (size_t index = 0; index < array_size; ++index) {
      current_alignment += padding +
        eprosima::fastcdr::Cdr::alignment(current_alignment, padding) +
        1;
    }
  }

  // Field name: height
  {
    size_t array_size = 1;
    last_member_size = array_size * sizeof(uint32_t);
    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }

  // Field name: width
  {
    size_t array_size = 1;
    last_member_size = array_size * sizeof(uint32_t);
    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }

  // Field name: keypoints_x
  {
    size_t array_size = 0;
    full_bounded = false;
    is_plain = false;
    current_alignment += padding +
      eprosima::fastcdr::Cdr::alignment(current_alignment, padding);
    last_member_size = array_size * sizeof(uint32_t);
    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }

  // Field name: keypoints_y
  {
    size_t array_size = 0;
    full_bounded = false;
    is_plain = false;
    current_alignment += padding +
      eprosima::fastcdr::Cdr::alignment(current_alignment, padding);
    last_member_size = array_size * sizeof(uint32_t);
    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }

  // Field name: keypoints_size
  {
    size_t array_size = 0;
    full_bounded = false;
    is_plain = false;
    current_alignment += padding +
      eprosima::fastcdr::Cdr::alignment(current_alignment, padding);
    last_member_size = array_size * sizeof(uint32_t);
    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }

  // Field name: keypoints_angle
  {
    size_t array_size = 0;
    full_bounded = false;
    is_plain = false;
    current_alignment += padding +
      eprosima::fastcdr::Cdr::alignment(current_alignment, padding);
    last_member_size = array_size * sizeof(uint32_t);
    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }

  // Field name: descriptors
  {
    size_t array_size = 0;
    full_bounded = false;
    is_plain = false;
    current_alignment += padding +
      eprosima::fastcdr::Cdr::alignment(current_alignment, padding);
    last_member_size = array_size * sizeof(uint32_t);
    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }

  // Field name: num_keypoints
  {
    size_t array_size = 1;
    last_member_size = array_size * sizeof(uint32_t);
    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }

  // Field name: publish_timestamp
  {
    size_t array_size = 1;
    last_member_size = array_size * sizeof(uint64_t);
    current_alignment += array_size * sizeof(uint64_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint64_t));
  }


  size_t ret_val = current_alignment - initial_alignment;
  if (is_plain) {
    // All members are plain, and type is not empty.
    // We still need to check that the in-memory alignment
    // is the same as the CDR mandated alignment.
    using DataType = sift_interfaces__msg__ImageFeatures;
    is_plain =
      (
      offsetof(DataType, publish_timestamp) +
      last_member_size
      ) == ret_val;
  }
  return ret_val;
}

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_sift_interfaces
bool cdr_serialize_key_sift_interfaces__msg__ImageFeatures(
  const sift_interfaces__msg__ImageFeatures * ros_message,
  eprosima::fastcdr::Cdr & cdr)
{
  // Field name: image_name
  {
    const rosidl_runtime_c__String * str = &ros_message->image_name;
    if (str->capacity == 0 || str->capacity <= str->size) {
      fprintf(stderr, "string capacity not greater than size\n");
      return false;
    }
    if (str->data[str->size] != '\0') {
      fprintf(stderr, "string not null-terminated\n");
      return false;
    }
    cdr << str->data;
  }

  // Field name: height
  {
    cdr << ros_message->height;
  }

  // Field name: width
  {
    cdr << ros_message->width;
  }

  // Field name: keypoints_x
  {
    size_t size = ros_message->keypoints_x.size;
    auto array_ptr = ros_message->keypoints_x.data;
    cdr << static_cast<uint32_t>(size);
    cdr.serialize_array(array_ptr, size);
  }

  // Field name: keypoints_y
  {
    size_t size = ros_message->keypoints_y.size;
    auto array_ptr = ros_message->keypoints_y.data;
    cdr << static_cast<uint32_t>(size);
    cdr.serialize_array(array_ptr, size);
  }

  // Field name: keypoints_size
  {
    size_t size = ros_message->keypoints_size.size;
    auto array_ptr = ros_message->keypoints_size.data;
    cdr << static_cast<uint32_t>(size);
    cdr.serialize_array(array_ptr, size);
  }

  // Field name: keypoints_angle
  {
    size_t size = ros_message->keypoints_angle.size;
    auto array_ptr = ros_message->keypoints_angle.data;
    cdr << static_cast<uint32_t>(size);
    cdr.serialize_array(array_ptr, size);
  }

  // Field name: descriptors
  {
    size_t size = ros_message->descriptors.size;
    auto array_ptr = ros_message->descriptors.data;
    cdr << static_cast<uint32_t>(size);
    cdr.serialize_array(array_ptr, size);
  }

  // Field name: num_keypoints
  {
    cdr << ros_message->num_keypoints;
  }

  // Field name: publish_timestamp
  {
    cdr << ros_message->publish_timestamp;
  }

  return true;
}

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_sift_interfaces
size_t get_serialized_size_key_sift_interfaces__msg__ImageFeatures(
  const void * untyped_ros_message,
  size_t current_alignment)
{
  const _ImageFeatures__ros_msg_type * ros_message = static_cast<const _ImageFeatures__ros_msg_type *>(untyped_ros_message);
  (void)ros_message;

  size_t initial_alignment = current_alignment;

  const size_t padding = 4;
  const size_t wchar_size = 4;
  (void)padding;
  (void)wchar_size;

  // Field name: image_name
  current_alignment += padding +
    eprosima::fastcdr::Cdr::alignment(current_alignment, padding) +
    (ros_message->image_name.size + 1);

  // Field name: height
  {
    size_t item_size = sizeof(ros_message->height);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }

  // Field name: width
  {
    size_t item_size = sizeof(ros_message->width);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }

  // Field name: keypoints_x
  {
    size_t array_size = ros_message->keypoints_x.size;
    auto array_ptr = ros_message->keypoints_x.data;
    current_alignment += padding +
      eprosima::fastcdr::Cdr::alignment(current_alignment, padding);
    (void)array_ptr;
    size_t item_size = sizeof(array_ptr[0]);
    current_alignment += array_size * item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }

  // Field name: keypoints_y
  {
    size_t array_size = ros_message->keypoints_y.size;
    auto array_ptr = ros_message->keypoints_y.data;
    current_alignment += padding +
      eprosima::fastcdr::Cdr::alignment(current_alignment, padding);
    (void)array_ptr;
    size_t item_size = sizeof(array_ptr[0]);
    current_alignment += array_size * item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }

  // Field name: keypoints_size
  {
    size_t array_size = ros_message->keypoints_size.size;
    auto array_ptr = ros_message->keypoints_size.data;
    current_alignment += padding +
      eprosima::fastcdr::Cdr::alignment(current_alignment, padding);
    (void)array_ptr;
    size_t item_size = sizeof(array_ptr[0]);
    current_alignment += array_size * item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }

  // Field name: keypoints_angle
  {
    size_t array_size = ros_message->keypoints_angle.size;
    auto array_ptr = ros_message->keypoints_angle.data;
    current_alignment += padding +
      eprosima::fastcdr::Cdr::alignment(current_alignment, padding);
    (void)array_ptr;
    size_t item_size = sizeof(array_ptr[0]);
    current_alignment += array_size * item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }

  // Field name: descriptors
  {
    size_t array_size = ros_message->descriptors.size;
    auto array_ptr = ros_message->descriptors.data;
    current_alignment += padding +
      eprosima::fastcdr::Cdr::alignment(current_alignment, padding);
    (void)array_ptr;
    size_t item_size = sizeof(array_ptr[0]);
    current_alignment += array_size * item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }

  // Field name: num_keypoints
  {
    size_t item_size = sizeof(ros_message->num_keypoints);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }

  // Field name: publish_timestamp
  {
    size_t item_size = sizeof(ros_message->publish_timestamp);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }

  return current_alignment - initial_alignment;
}

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_sift_interfaces
size_t max_serialized_size_key_sift_interfaces__msg__ImageFeatures(
  bool & full_bounded,
  bool & is_plain,
  size_t current_alignment)
{
  size_t initial_alignment = current_alignment;

  const size_t padding = 4;
  const size_t wchar_size = 4;
  size_t last_member_size = 0;
  (void)last_member_size;
  (void)padding;
  (void)wchar_size;

  full_bounded = true;
  is_plain = true;
  // Field name: image_name
  {
    size_t array_size = 1;
    full_bounded = false;
    is_plain = false;
    for (size_t index = 0; index < array_size; ++index) {
      current_alignment += padding +
        eprosima::fastcdr::Cdr::alignment(current_alignment, padding) +
        1;
    }
  }

  // Field name: height
  {
    size_t array_size = 1;
    last_member_size = array_size * sizeof(uint32_t);
    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }

  // Field name: width
  {
    size_t array_size = 1;
    last_member_size = array_size * sizeof(uint32_t);
    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }

  // Field name: keypoints_x
  {
    size_t array_size = 0;
    full_bounded = false;
    is_plain = false;
    current_alignment += padding +
      eprosima::fastcdr::Cdr::alignment(current_alignment, padding);
    last_member_size = array_size * sizeof(uint32_t);
    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }

  // Field name: keypoints_y
  {
    size_t array_size = 0;
    full_bounded = false;
    is_plain = false;
    current_alignment += padding +
      eprosima::fastcdr::Cdr::alignment(current_alignment, padding);
    last_member_size = array_size * sizeof(uint32_t);
    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }

  // Field name: keypoints_size
  {
    size_t array_size = 0;
    full_bounded = false;
    is_plain = false;
    current_alignment += padding +
      eprosima::fastcdr::Cdr::alignment(current_alignment, padding);
    last_member_size = array_size * sizeof(uint32_t);
    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }

  // Field name: keypoints_angle
  {
    size_t array_size = 0;
    full_bounded = false;
    is_plain = false;
    current_alignment += padding +
      eprosima::fastcdr::Cdr::alignment(current_alignment, padding);
    last_member_size = array_size * sizeof(uint32_t);
    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }

  // Field name: descriptors
  {
    size_t array_size = 0;
    full_bounded = false;
    is_plain = false;
    current_alignment += padding +
      eprosima::fastcdr::Cdr::alignment(current_alignment, padding);
    last_member_size = array_size * sizeof(uint32_t);
    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }

  // Field name: num_keypoints
  {
    size_t array_size = 1;
    last_member_size = array_size * sizeof(uint32_t);
    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }

  // Field name: publish_timestamp
  {
    size_t array_size = 1;
    last_member_size = array_size * sizeof(uint64_t);
    current_alignment += array_size * sizeof(uint64_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint64_t));
  }

  size_t ret_val = current_alignment - initial_alignment;
  if (is_plain) {
    // All members are plain, and type is not empty.
    // We still need to check that the in-memory alignment
    // is the same as the CDR mandated alignment.
    using DataType = sift_interfaces__msg__ImageFeatures;
    is_plain =
      (
      offsetof(DataType, publish_timestamp) +
      last_member_size
      ) == ret_val;
  }
  return ret_val;
}


static bool _ImageFeatures__cdr_serialize(
  const void * untyped_ros_message,
  eprosima::fastcdr::Cdr & cdr)
{
  if (!untyped_ros_message) {
    fprintf(stderr, "ros message handle is null\n");
    return false;
  }
  const sift_interfaces__msg__ImageFeatures * ros_message = static_cast<const sift_interfaces__msg__ImageFeatures *>(untyped_ros_message);
  (void)ros_message;
  return cdr_serialize_sift_interfaces__msg__ImageFeatures(ros_message, cdr);
}

static bool _ImageFeatures__cdr_deserialize(
  eprosima::fastcdr::Cdr & cdr,
  void * untyped_ros_message)
{
  if (!untyped_ros_message) {
    fprintf(stderr, "ros message handle is null\n");
    return false;
  }
  sift_interfaces__msg__ImageFeatures * ros_message = static_cast<sift_interfaces__msg__ImageFeatures *>(untyped_ros_message);
  (void)ros_message;
  return cdr_deserialize_sift_interfaces__msg__ImageFeatures(cdr, ros_message);
}

static uint32_t _ImageFeatures__get_serialized_size(const void * untyped_ros_message)
{
  return static_cast<uint32_t>(
    get_serialized_size_sift_interfaces__msg__ImageFeatures(
      untyped_ros_message, 0));
}

static size_t _ImageFeatures__max_serialized_size(char & bounds_info)
{
  bool full_bounded;
  bool is_plain;
  size_t ret_val;

  ret_val = max_serialized_size_sift_interfaces__msg__ImageFeatures(
    full_bounded, is_plain, 0);

  bounds_info =
    is_plain ? ROSIDL_TYPESUPPORT_FASTRTPS_PLAIN_TYPE :
    full_bounded ? ROSIDL_TYPESUPPORT_FASTRTPS_BOUNDED_TYPE : ROSIDL_TYPESUPPORT_FASTRTPS_UNBOUNDED_TYPE;
  return ret_val;
}


static message_type_support_callbacks_t __callbacks_ImageFeatures = {
  "sift_interfaces::msg",
  "ImageFeatures",
  _ImageFeatures__cdr_serialize,
  _ImageFeatures__cdr_deserialize,
  _ImageFeatures__get_serialized_size,
  _ImageFeatures__max_serialized_size,
  nullptr
};

static rosidl_message_type_support_t _ImageFeatures__type_support = {
  rosidl_typesupport_fastrtps_c__identifier,
  &__callbacks_ImageFeatures,
  get_message_typesupport_handle_function,
  &sift_interfaces__msg__ImageFeatures__get_type_hash,
  &sift_interfaces__msg__ImageFeatures__get_type_description,
  &sift_interfaces__msg__ImageFeatures__get_type_description_sources,
};

const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_c, sift_interfaces, msg, ImageFeatures)() {
  return &_ImageFeatures__type_support;
}

#if defined(__cplusplus)
}
#endif
