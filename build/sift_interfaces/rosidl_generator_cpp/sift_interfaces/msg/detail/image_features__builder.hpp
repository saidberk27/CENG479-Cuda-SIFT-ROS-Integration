// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from sift_interfaces:msg/ImageFeatures.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "sift_interfaces/msg/image_features.hpp"


#ifndef SIFT_INTERFACES__MSG__DETAIL__IMAGE_FEATURES__BUILDER_HPP_
#define SIFT_INTERFACES__MSG__DETAIL__IMAGE_FEATURES__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "sift_interfaces/msg/detail/image_features__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace sift_interfaces
{

namespace msg
{

namespace builder
{

class Init_ImageFeatures_publish_timestamp
{
public:
  explicit Init_ImageFeatures_publish_timestamp(::sift_interfaces::msg::ImageFeatures & msg)
  : msg_(msg)
  {}
  ::sift_interfaces::msg::ImageFeatures publish_timestamp(::sift_interfaces::msg::ImageFeatures::_publish_timestamp_type arg)
  {
    msg_.publish_timestamp = std::move(arg);
    return std::move(msg_);
  }

private:
  ::sift_interfaces::msg::ImageFeatures msg_;
};

class Init_ImageFeatures_num_keypoints
{
public:
  explicit Init_ImageFeatures_num_keypoints(::sift_interfaces::msg::ImageFeatures & msg)
  : msg_(msg)
  {}
  Init_ImageFeatures_publish_timestamp num_keypoints(::sift_interfaces::msg::ImageFeatures::_num_keypoints_type arg)
  {
    msg_.num_keypoints = std::move(arg);
    return Init_ImageFeatures_publish_timestamp(msg_);
  }

private:
  ::sift_interfaces::msg::ImageFeatures msg_;
};

class Init_ImageFeatures_descriptors
{
public:
  explicit Init_ImageFeatures_descriptors(::sift_interfaces::msg::ImageFeatures & msg)
  : msg_(msg)
  {}
  Init_ImageFeatures_num_keypoints descriptors(::sift_interfaces::msg::ImageFeatures::_descriptors_type arg)
  {
    msg_.descriptors = std::move(arg);
    return Init_ImageFeatures_num_keypoints(msg_);
  }

private:
  ::sift_interfaces::msg::ImageFeatures msg_;
};

class Init_ImageFeatures_keypoints_angle
{
public:
  explicit Init_ImageFeatures_keypoints_angle(::sift_interfaces::msg::ImageFeatures & msg)
  : msg_(msg)
  {}
  Init_ImageFeatures_descriptors keypoints_angle(::sift_interfaces::msg::ImageFeatures::_keypoints_angle_type arg)
  {
    msg_.keypoints_angle = std::move(arg);
    return Init_ImageFeatures_descriptors(msg_);
  }

private:
  ::sift_interfaces::msg::ImageFeatures msg_;
};

class Init_ImageFeatures_keypoints_size
{
public:
  explicit Init_ImageFeatures_keypoints_size(::sift_interfaces::msg::ImageFeatures & msg)
  : msg_(msg)
  {}
  Init_ImageFeatures_keypoints_angle keypoints_size(::sift_interfaces::msg::ImageFeatures::_keypoints_size_type arg)
  {
    msg_.keypoints_size = std::move(arg);
    return Init_ImageFeatures_keypoints_angle(msg_);
  }

private:
  ::sift_interfaces::msg::ImageFeatures msg_;
};

class Init_ImageFeatures_keypoints_y
{
public:
  explicit Init_ImageFeatures_keypoints_y(::sift_interfaces::msg::ImageFeatures & msg)
  : msg_(msg)
  {}
  Init_ImageFeatures_keypoints_size keypoints_y(::sift_interfaces::msg::ImageFeatures::_keypoints_y_type arg)
  {
    msg_.keypoints_y = std::move(arg);
    return Init_ImageFeatures_keypoints_size(msg_);
  }

private:
  ::sift_interfaces::msg::ImageFeatures msg_;
};

class Init_ImageFeatures_keypoints_x
{
public:
  explicit Init_ImageFeatures_keypoints_x(::sift_interfaces::msg::ImageFeatures & msg)
  : msg_(msg)
  {}
  Init_ImageFeatures_keypoints_y keypoints_x(::sift_interfaces::msg::ImageFeatures::_keypoints_x_type arg)
  {
    msg_.keypoints_x = std::move(arg);
    return Init_ImageFeatures_keypoints_y(msg_);
  }

private:
  ::sift_interfaces::msg::ImageFeatures msg_;
};

class Init_ImageFeatures_width
{
public:
  explicit Init_ImageFeatures_width(::sift_interfaces::msg::ImageFeatures & msg)
  : msg_(msg)
  {}
  Init_ImageFeatures_keypoints_x width(::sift_interfaces::msg::ImageFeatures::_width_type arg)
  {
    msg_.width = std::move(arg);
    return Init_ImageFeatures_keypoints_x(msg_);
  }

private:
  ::sift_interfaces::msg::ImageFeatures msg_;
};

class Init_ImageFeatures_height
{
public:
  explicit Init_ImageFeatures_height(::sift_interfaces::msg::ImageFeatures & msg)
  : msg_(msg)
  {}
  Init_ImageFeatures_width height(::sift_interfaces::msg::ImageFeatures::_height_type arg)
  {
    msg_.height = std::move(arg);
    return Init_ImageFeatures_width(msg_);
  }

private:
  ::sift_interfaces::msg::ImageFeatures msg_;
};

class Init_ImageFeatures_image_name
{
public:
  Init_ImageFeatures_image_name()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_ImageFeatures_height image_name(::sift_interfaces::msg::ImageFeatures::_image_name_type arg)
  {
    msg_.image_name = std::move(arg);
    return Init_ImageFeatures_height(msg_);
  }

private:
  ::sift_interfaces::msg::ImageFeatures msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::sift_interfaces::msg::ImageFeatures>()
{
  return sift_interfaces::msg::builder::Init_ImageFeatures_image_name();
}

}  // namespace sift_interfaces

#endif  // SIFT_INTERFACES__MSG__DETAIL__IMAGE_FEATURES__BUILDER_HPP_
