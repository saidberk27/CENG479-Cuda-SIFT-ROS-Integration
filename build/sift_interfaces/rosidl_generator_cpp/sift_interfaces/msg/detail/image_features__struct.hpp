// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from sift_interfaces:msg/ImageFeatures.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "sift_interfaces/msg/image_features.hpp"


#ifndef SIFT_INTERFACES__MSG__DETAIL__IMAGE_FEATURES__STRUCT_HPP_
#define SIFT_INTERFACES__MSG__DETAIL__IMAGE_FEATURES__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


#ifndef _WIN32
# define DEPRECATED__sift_interfaces__msg__ImageFeatures __attribute__((deprecated))
#else
# define DEPRECATED__sift_interfaces__msg__ImageFeatures __declspec(deprecated)
#endif

namespace sift_interfaces
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct ImageFeatures_
{
  using Type = ImageFeatures_<ContainerAllocator>;

  explicit ImageFeatures_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->image_name = "";
      this->height = 0l;
      this->width = 0l;
      this->num_keypoints = 0l;
      this->publish_timestamp = 0.0;
    }
  }

  explicit ImageFeatures_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : image_name(_alloc)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->image_name = "";
      this->height = 0l;
      this->width = 0l;
      this->num_keypoints = 0l;
      this->publish_timestamp = 0.0;
    }
  }

  // field types and members
  using _image_name_type =
    std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>>;
  _image_name_type image_name;
  using _height_type =
    int32_t;
  _height_type height;
  using _width_type =
    int32_t;
  _width_type width;
  using _keypoints_x_type =
    std::vector<float, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<float>>;
  _keypoints_x_type keypoints_x;
  using _keypoints_y_type =
    std::vector<float, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<float>>;
  _keypoints_y_type keypoints_y;
  using _keypoints_size_type =
    std::vector<float, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<float>>;
  _keypoints_size_type keypoints_size;
  using _keypoints_angle_type =
    std::vector<float, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<float>>;
  _keypoints_angle_type keypoints_angle;
  using _descriptors_type =
    std::vector<float, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<float>>;
  _descriptors_type descriptors;
  using _num_keypoints_type =
    int32_t;
  _num_keypoints_type num_keypoints;
  using _publish_timestamp_type =
    double;
  _publish_timestamp_type publish_timestamp;

  // setters for named parameter idiom
  Type & set__image_name(
    const std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>> & _arg)
  {
    this->image_name = _arg;
    return *this;
  }
  Type & set__height(
    const int32_t & _arg)
  {
    this->height = _arg;
    return *this;
  }
  Type & set__width(
    const int32_t & _arg)
  {
    this->width = _arg;
    return *this;
  }
  Type & set__keypoints_x(
    const std::vector<float, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<float>> & _arg)
  {
    this->keypoints_x = _arg;
    return *this;
  }
  Type & set__keypoints_y(
    const std::vector<float, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<float>> & _arg)
  {
    this->keypoints_y = _arg;
    return *this;
  }
  Type & set__keypoints_size(
    const std::vector<float, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<float>> & _arg)
  {
    this->keypoints_size = _arg;
    return *this;
  }
  Type & set__keypoints_angle(
    const std::vector<float, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<float>> & _arg)
  {
    this->keypoints_angle = _arg;
    return *this;
  }
  Type & set__descriptors(
    const std::vector<float, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<float>> & _arg)
  {
    this->descriptors = _arg;
    return *this;
  }
  Type & set__num_keypoints(
    const int32_t & _arg)
  {
    this->num_keypoints = _arg;
    return *this;
  }
  Type & set__publish_timestamp(
    const double & _arg)
  {
    this->publish_timestamp = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    sift_interfaces::msg::ImageFeatures_<ContainerAllocator> *;
  using ConstRawPtr =
    const sift_interfaces::msg::ImageFeatures_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<sift_interfaces::msg::ImageFeatures_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<sift_interfaces::msg::ImageFeatures_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      sift_interfaces::msg::ImageFeatures_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<sift_interfaces::msg::ImageFeatures_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      sift_interfaces::msg::ImageFeatures_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<sift_interfaces::msg::ImageFeatures_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<sift_interfaces::msg::ImageFeatures_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<sift_interfaces::msg::ImageFeatures_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__sift_interfaces__msg__ImageFeatures
    std::shared_ptr<sift_interfaces::msg::ImageFeatures_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__sift_interfaces__msg__ImageFeatures
    std::shared_ptr<sift_interfaces::msg::ImageFeatures_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const ImageFeatures_ & other) const
  {
    if (this->image_name != other.image_name) {
      return false;
    }
    if (this->height != other.height) {
      return false;
    }
    if (this->width != other.width) {
      return false;
    }
    if (this->keypoints_x != other.keypoints_x) {
      return false;
    }
    if (this->keypoints_y != other.keypoints_y) {
      return false;
    }
    if (this->keypoints_size != other.keypoints_size) {
      return false;
    }
    if (this->keypoints_angle != other.keypoints_angle) {
      return false;
    }
    if (this->descriptors != other.descriptors) {
      return false;
    }
    if (this->num_keypoints != other.num_keypoints) {
      return false;
    }
    if (this->publish_timestamp != other.publish_timestamp) {
      return false;
    }
    return true;
  }
  bool operator!=(const ImageFeatures_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct ImageFeatures_

// alias to use template instance with default allocator
using ImageFeatures =
  sift_interfaces::msg::ImageFeatures_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace sift_interfaces

#endif  // SIFT_INTERFACES__MSG__DETAIL__IMAGE_FEATURES__STRUCT_HPP_
