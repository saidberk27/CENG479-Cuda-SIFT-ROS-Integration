// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from sift_interfaces:msg/ImageFeatures.idl
// generated code does not contain a copyright notice
#include "sift_interfaces/msg/detail/image_features__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


// Include directives for member types
// Member `image_name`
#include "rosidl_runtime_c/string_functions.h"
// Member `keypoints_x`
// Member `keypoints_y`
// Member `keypoints_size`
// Member `keypoints_angle`
// Member `descriptors`
#include "rosidl_runtime_c/primitives_sequence_functions.h"

bool
sift_interfaces__msg__ImageFeatures__init(sift_interfaces__msg__ImageFeatures * msg)
{
  if (!msg) {
    return false;
  }
  // image_name
  if (!rosidl_runtime_c__String__init(&msg->image_name)) {
    sift_interfaces__msg__ImageFeatures__fini(msg);
    return false;
  }
  // height
  // width
  // keypoints_x
  if (!rosidl_runtime_c__float__Sequence__init(&msg->keypoints_x, 0)) {
    sift_interfaces__msg__ImageFeatures__fini(msg);
    return false;
  }
  // keypoints_y
  if (!rosidl_runtime_c__float__Sequence__init(&msg->keypoints_y, 0)) {
    sift_interfaces__msg__ImageFeatures__fini(msg);
    return false;
  }
  // keypoints_size
  if (!rosidl_runtime_c__float__Sequence__init(&msg->keypoints_size, 0)) {
    sift_interfaces__msg__ImageFeatures__fini(msg);
    return false;
  }
  // keypoints_angle
  if (!rosidl_runtime_c__float__Sequence__init(&msg->keypoints_angle, 0)) {
    sift_interfaces__msg__ImageFeatures__fini(msg);
    return false;
  }
  // descriptors
  if (!rosidl_runtime_c__float__Sequence__init(&msg->descriptors, 0)) {
    sift_interfaces__msg__ImageFeatures__fini(msg);
    return false;
  }
  // num_keypoints
  // publish_timestamp
  return true;
}

void
sift_interfaces__msg__ImageFeatures__fini(sift_interfaces__msg__ImageFeatures * msg)
{
  if (!msg) {
    return;
  }
  // image_name
  rosidl_runtime_c__String__fini(&msg->image_name);
  // height
  // width
  // keypoints_x
  rosidl_runtime_c__float__Sequence__fini(&msg->keypoints_x);
  // keypoints_y
  rosidl_runtime_c__float__Sequence__fini(&msg->keypoints_y);
  // keypoints_size
  rosidl_runtime_c__float__Sequence__fini(&msg->keypoints_size);
  // keypoints_angle
  rosidl_runtime_c__float__Sequence__fini(&msg->keypoints_angle);
  // descriptors
  rosidl_runtime_c__float__Sequence__fini(&msg->descriptors);
  // num_keypoints
  // publish_timestamp
}

bool
sift_interfaces__msg__ImageFeatures__are_equal(const sift_interfaces__msg__ImageFeatures * lhs, const sift_interfaces__msg__ImageFeatures * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // image_name
  if (!rosidl_runtime_c__String__are_equal(
      &(lhs->image_name), &(rhs->image_name)))
  {
    return false;
  }
  // height
  if (lhs->height != rhs->height) {
    return false;
  }
  // width
  if (lhs->width != rhs->width) {
    return false;
  }
  // keypoints_x
  if (!rosidl_runtime_c__float__Sequence__are_equal(
      &(lhs->keypoints_x), &(rhs->keypoints_x)))
  {
    return false;
  }
  // keypoints_y
  if (!rosidl_runtime_c__float__Sequence__are_equal(
      &(lhs->keypoints_y), &(rhs->keypoints_y)))
  {
    return false;
  }
  // keypoints_size
  if (!rosidl_runtime_c__float__Sequence__are_equal(
      &(lhs->keypoints_size), &(rhs->keypoints_size)))
  {
    return false;
  }
  // keypoints_angle
  if (!rosidl_runtime_c__float__Sequence__are_equal(
      &(lhs->keypoints_angle), &(rhs->keypoints_angle)))
  {
    return false;
  }
  // descriptors
  if (!rosidl_runtime_c__float__Sequence__are_equal(
      &(lhs->descriptors), &(rhs->descriptors)))
  {
    return false;
  }
  // num_keypoints
  if (lhs->num_keypoints != rhs->num_keypoints) {
    return false;
  }
  // publish_timestamp
  if (lhs->publish_timestamp != rhs->publish_timestamp) {
    return false;
  }
  return true;
}

bool
sift_interfaces__msg__ImageFeatures__copy(
  const sift_interfaces__msg__ImageFeatures * input,
  sift_interfaces__msg__ImageFeatures * output)
{
  if (!input || !output) {
    return false;
  }
  // image_name
  if (!rosidl_runtime_c__String__copy(
      &(input->image_name), &(output->image_name)))
  {
    return false;
  }
  // height
  output->height = input->height;
  // width
  output->width = input->width;
  // keypoints_x
  if (!rosidl_runtime_c__float__Sequence__copy(
      &(input->keypoints_x), &(output->keypoints_x)))
  {
    return false;
  }
  // keypoints_y
  if (!rosidl_runtime_c__float__Sequence__copy(
      &(input->keypoints_y), &(output->keypoints_y)))
  {
    return false;
  }
  // keypoints_size
  if (!rosidl_runtime_c__float__Sequence__copy(
      &(input->keypoints_size), &(output->keypoints_size)))
  {
    return false;
  }
  // keypoints_angle
  if (!rosidl_runtime_c__float__Sequence__copy(
      &(input->keypoints_angle), &(output->keypoints_angle)))
  {
    return false;
  }
  // descriptors
  if (!rosidl_runtime_c__float__Sequence__copy(
      &(input->descriptors), &(output->descriptors)))
  {
    return false;
  }
  // num_keypoints
  output->num_keypoints = input->num_keypoints;
  // publish_timestamp
  output->publish_timestamp = input->publish_timestamp;
  return true;
}

sift_interfaces__msg__ImageFeatures *
sift_interfaces__msg__ImageFeatures__create(void)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  sift_interfaces__msg__ImageFeatures * msg = (sift_interfaces__msg__ImageFeatures *)allocator.allocate(sizeof(sift_interfaces__msg__ImageFeatures), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(sift_interfaces__msg__ImageFeatures));
  bool success = sift_interfaces__msg__ImageFeatures__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
sift_interfaces__msg__ImageFeatures__destroy(sift_interfaces__msg__ImageFeatures * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    sift_interfaces__msg__ImageFeatures__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
sift_interfaces__msg__ImageFeatures__Sequence__init(sift_interfaces__msg__ImageFeatures__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  sift_interfaces__msg__ImageFeatures * data = NULL;

  if (size) {
    data = (sift_interfaces__msg__ImageFeatures *)allocator.zero_allocate(size, sizeof(sift_interfaces__msg__ImageFeatures), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = sift_interfaces__msg__ImageFeatures__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        sift_interfaces__msg__ImageFeatures__fini(&data[i - 1]);
      }
      allocator.deallocate(data, allocator.state);
      return false;
    }
  }
  array->data = data;
  array->size = size;
  array->capacity = size;
  return true;
}

void
sift_interfaces__msg__ImageFeatures__Sequence__fini(sift_interfaces__msg__ImageFeatures__Sequence * array)
{
  if (!array) {
    return;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();

  if (array->data) {
    // ensure that data and capacity values are consistent
    assert(array->capacity > 0);
    // finalize all array elements
    for (size_t i = 0; i < array->capacity; ++i) {
      sift_interfaces__msg__ImageFeatures__fini(&array->data[i]);
    }
    allocator.deallocate(array->data, allocator.state);
    array->data = NULL;
    array->size = 0;
    array->capacity = 0;
  } else {
    // ensure that data, size, and capacity values are consistent
    assert(0 == array->size);
    assert(0 == array->capacity);
  }
}

sift_interfaces__msg__ImageFeatures__Sequence *
sift_interfaces__msg__ImageFeatures__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  sift_interfaces__msg__ImageFeatures__Sequence * array = (sift_interfaces__msg__ImageFeatures__Sequence *)allocator.allocate(sizeof(sift_interfaces__msg__ImageFeatures__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = sift_interfaces__msg__ImageFeatures__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
sift_interfaces__msg__ImageFeatures__Sequence__destroy(sift_interfaces__msg__ImageFeatures__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    sift_interfaces__msg__ImageFeatures__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
sift_interfaces__msg__ImageFeatures__Sequence__are_equal(const sift_interfaces__msg__ImageFeatures__Sequence * lhs, const sift_interfaces__msg__ImageFeatures__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!sift_interfaces__msg__ImageFeatures__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
sift_interfaces__msg__ImageFeatures__Sequence__copy(
  const sift_interfaces__msg__ImageFeatures__Sequence * input,
  sift_interfaces__msg__ImageFeatures__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(sift_interfaces__msg__ImageFeatures);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    sift_interfaces__msg__ImageFeatures * data =
      (sift_interfaces__msg__ImageFeatures *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!sift_interfaces__msg__ImageFeatures__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          sift_interfaces__msg__ImageFeatures__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!sift_interfaces__msg__ImageFeatures__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
