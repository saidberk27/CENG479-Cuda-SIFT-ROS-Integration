// generated from rosidl_generator_c/resource/idl__functions.h.em
// with input from sift_interfaces:msg/ImageFeatures.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "sift_interfaces/msg/image_features.h"


#ifndef SIFT_INTERFACES__MSG__DETAIL__IMAGE_FEATURES__FUNCTIONS_H_
#define SIFT_INTERFACES__MSG__DETAIL__IMAGE_FEATURES__FUNCTIONS_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stdlib.h>

#include "rosidl_runtime_c/action_type_support_struct.h"
#include "rosidl_runtime_c/message_type_support_struct.h"
#include "rosidl_runtime_c/service_type_support_struct.h"
#include "rosidl_runtime_c/type_description/type_description__struct.h"
#include "rosidl_runtime_c/type_description/type_source__struct.h"
#include "rosidl_runtime_c/type_hash.h"
#include "rosidl_runtime_c/visibility_control.h"
#include "sift_interfaces/msg/rosidl_generator_c__visibility_control.h"

#include "sift_interfaces/msg/detail/image_features__struct.h"

/// Initialize msg/ImageFeatures message.
/**
 * If the init function is called twice for the same message without
 * calling fini inbetween previously allocated memory will be leaked.
 * \param[in,out] msg The previously allocated message pointer.
 * Fields without a default value will not be initialized by this function.
 * You might want to call memset(msg, 0, sizeof(
 * sift_interfaces__msg__ImageFeatures
 * )) before or use
 * sift_interfaces__msg__ImageFeatures__create()
 * to allocate and initialize the message.
 * \return true if initialization was successful, otherwise false
 */
ROSIDL_GENERATOR_C_PUBLIC_sift_interfaces
bool
sift_interfaces__msg__ImageFeatures__init(sift_interfaces__msg__ImageFeatures * msg);

/// Finalize msg/ImageFeatures message.
/**
 * \param[in,out] msg The allocated message pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_sift_interfaces
void
sift_interfaces__msg__ImageFeatures__fini(sift_interfaces__msg__ImageFeatures * msg);

/// Create msg/ImageFeatures message.
/**
 * It allocates the memory for the message, sets the memory to zero, and
 * calls
 * sift_interfaces__msg__ImageFeatures__init().
 * \return The pointer to the initialized message if successful,
 * otherwise NULL
 */
ROSIDL_GENERATOR_C_PUBLIC_sift_interfaces
sift_interfaces__msg__ImageFeatures *
sift_interfaces__msg__ImageFeatures__create(void);

/// Destroy msg/ImageFeatures message.
/**
 * It calls
 * sift_interfaces__msg__ImageFeatures__fini()
 * and frees the memory of the message.
 * \param[in,out] msg The allocated message pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_sift_interfaces
void
sift_interfaces__msg__ImageFeatures__destroy(sift_interfaces__msg__ImageFeatures * msg);

/// Check for msg/ImageFeatures message equality.
/**
 * \param[in] lhs The message on the left hand size of the equality operator.
 * \param[in] rhs The message on the right hand size of the equality operator.
 * \return true if messages are equal, otherwise false.
 */
ROSIDL_GENERATOR_C_PUBLIC_sift_interfaces
bool
sift_interfaces__msg__ImageFeatures__are_equal(const sift_interfaces__msg__ImageFeatures * lhs, const sift_interfaces__msg__ImageFeatures * rhs);

/// Copy a msg/ImageFeatures message.
/**
 * This functions performs a deep copy, as opposed to the shallow copy that
 * plain assignment yields.
 *
 * \param[in] input The source message pointer.
 * \param[out] output The target message pointer, which must
 *   have been initialized before calling this function.
 * \return true if successful, or false if either pointer is null
 *   or memory allocation fails.
 */
ROSIDL_GENERATOR_C_PUBLIC_sift_interfaces
bool
sift_interfaces__msg__ImageFeatures__copy(
  const sift_interfaces__msg__ImageFeatures * input,
  sift_interfaces__msg__ImageFeatures * output);

/// Retrieve pointer to the hash of the description of this type.
ROSIDL_GENERATOR_C_PUBLIC_sift_interfaces
const rosidl_type_hash_t *
sift_interfaces__msg__ImageFeatures__get_type_hash(
  const rosidl_message_type_support_t * type_support);

/// Retrieve pointer to the description of this type.
ROSIDL_GENERATOR_C_PUBLIC_sift_interfaces
const rosidl_runtime_c__type_description__TypeDescription *
sift_interfaces__msg__ImageFeatures__get_type_description(
  const rosidl_message_type_support_t * type_support);

/// Retrieve pointer to the single raw source text that defined this type.
ROSIDL_GENERATOR_C_PUBLIC_sift_interfaces
const rosidl_runtime_c__type_description__TypeSource *
sift_interfaces__msg__ImageFeatures__get_individual_type_description_source(
  const rosidl_message_type_support_t * type_support);

/// Retrieve pointer to the recursive raw sources that defined the description of this type.
ROSIDL_GENERATOR_C_PUBLIC_sift_interfaces
const rosidl_runtime_c__type_description__TypeSource__Sequence *
sift_interfaces__msg__ImageFeatures__get_type_description_sources(
  const rosidl_message_type_support_t * type_support);

/// Initialize array of msg/ImageFeatures messages.
/**
 * It allocates the memory for the number of elements and calls
 * sift_interfaces__msg__ImageFeatures__init()
 * for each element of the array.
 * \param[in,out] array The allocated array pointer.
 * \param[in] size The size / capacity of the array.
 * \return true if initialization was successful, otherwise false
 * If the array pointer is valid and the size is zero it is guaranteed
 # to return true.
 */
ROSIDL_GENERATOR_C_PUBLIC_sift_interfaces
bool
sift_interfaces__msg__ImageFeatures__Sequence__init(sift_interfaces__msg__ImageFeatures__Sequence * array, size_t size);

/// Finalize array of msg/ImageFeatures messages.
/**
 * It calls
 * sift_interfaces__msg__ImageFeatures__fini()
 * for each element of the array and frees the memory for the number of
 * elements.
 * \param[in,out] array The initialized array pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_sift_interfaces
void
sift_interfaces__msg__ImageFeatures__Sequence__fini(sift_interfaces__msg__ImageFeatures__Sequence * array);

/// Create array of msg/ImageFeatures messages.
/**
 * It allocates the memory for the array and calls
 * sift_interfaces__msg__ImageFeatures__Sequence__init().
 * \param[in] size The size / capacity of the array.
 * \return The pointer to the initialized array if successful, otherwise NULL
 */
ROSIDL_GENERATOR_C_PUBLIC_sift_interfaces
sift_interfaces__msg__ImageFeatures__Sequence *
sift_interfaces__msg__ImageFeatures__Sequence__create(size_t size);

/// Destroy array of msg/ImageFeatures messages.
/**
 * It calls
 * sift_interfaces__msg__ImageFeatures__Sequence__fini()
 * on the array,
 * and frees the memory of the array.
 * \param[in,out] array The initialized array pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_sift_interfaces
void
sift_interfaces__msg__ImageFeatures__Sequence__destroy(sift_interfaces__msg__ImageFeatures__Sequence * array);

/// Check for msg/ImageFeatures message array equality.
/**
 * \param[in] lhs The message array on the left hand size of the equality operator.
 * \param[in] rhs The message array on the right hand size of the equality operator.
 * \return true if message arrays are equal in size and content, otherwise false.
 */
ROSIDL_GENERATOR_C_PUBLIC_sift_interfaces
bool
sift_interfaces__msg__ImageFeatures__Sequence__are_equal(const sift_interfaces__msg__ImageFeatures__Sequence * lhs, const sift_interfaces__msg__ImageFeatures__Sequence * rhs);

/// Copy an array of msg/ImageFeatures messages.
/**
 * This functions performs a deep copy, as opposed to the shallow copy that
 * plain assignment yields.
 *
 * \param[in] input The source array pointer.
 * \param[out] output The target array pointer, which must
 *   have been initialized before calling this function.
 * \return true if successful, or false if either pointer
 *   is null or memory allocation fails.
 */
ROSIDL_GENERATOR_C_PUBLIC_sift_interfaces
bool
sift_interfaces__msg__ImageFeatures__Sequence__copy(
  const sift_interfaces__msg__ImageFeatures__Sequence * input,
  sift_interfaces__msg__ImageFeatures__Sequence * output);

#ifdef __cplusplus
}
#endif

#endif  // SIFT_INTERFACES__MSG__DETAIL__IMAGE_FEATURES__FUNCTIONS_H_
