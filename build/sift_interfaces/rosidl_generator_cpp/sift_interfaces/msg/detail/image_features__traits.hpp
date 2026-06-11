// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from sift_interfaces:msg/ImageFeatures.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "sift_interfaces/msg/image_features.hpp"


#ifndef SIFT_INTERFACES__MSG__DETAIL__IMAGE_FEATURES__TRAITS_HPP_
#define SIFT_INTERFACES__MSG__DETAIL__IMAGE_FEATURES__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "sift_interfaces/msg/detail/image_features__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

namespace sift_interfaces
{

namespace msg
{

inline void to_flow_style_yaml(
  const ImageFeatures & msg,
  std::ostream & out)
{
  out << "{";
  // member: image_name
  {
    out << "image_name: ";
    rosidl_generator_traits::value_to_yaml(msg.image_name, out);
    out << ", ";
  }

  // member: height
  {
    out << "height: ";
    rosidl_generator_traits::value_to_yaml(msg.height, out);
    out << ", ";
  }

  // member: width
  {
    out << "width: ";
    rosidl_generator_traits::value_to_yaml(msg.width, out);
    out << ", ";
  }

  // member: keypoints_x
  {
    if (msg.keypoints_x.size() == 0) {
      out << "keypoints_x: []";
    } else {
      out << "keypoints_x: [";
      size_t pending_items = msg.keypoints_x.size();
      for (auto item : msg.keypoints_x) {
        rosidl_generator_traits::value_to_yaml(item, out);
        if (--pending_items > 0) {
          out << ", ";
        }
      }
      out << "]";
    }
    out << ", ";
  }

  // member: keypoints_y
  {
    if (msg.keypoints_y.size() == 0) {
      out << "keypoints_y: []";
    } else {
      out << "keypoints_y: [";
      size_t pending_items = msg.keypoints_y.size();
      for (auto item : msg.keypoints_y) {
        rosidl_generator_traits::value_to_yaml(item, out);
        if (--pending_items > 0) {
          out << ", ";
        }
      }
      out << "]";
    }
    out << ", ";
  }

  // member: keypoints_size
  {
    if (msg.keypoints_size.size() == 0) {
      out << "keypoints_size: []";
    } else {
      out << "keypoints_size: [";
      size_t pending_items = msg.keypoints_size.size();
      for (auto item : msg.keypoints_size) {
        rosidl_generator_traits::value_to_yaml(item, out);
        if (--pending_items > 0) {
          out << ", ";
        }
      }
      out << "]";
    }
    out << ", ";
  }

  // member: keypoints_angle
  {
    if (msg.keypoints_angle.size() == 0) {
      out << "keypoints_angle: []";
    } else {
      out << "keypoints_angle: [";
      size_t pending_items = msg.keypoints_angle.size();
      for (auto item : msg.keypoints_angle) {
        rosidl_generator_traits::value_to_yaml(item, out);
        if (--pending_items > 0) {
          out << ", ";
        }
      }
      out << "]";
    }
    out << ", ";
  }

  // member: descriptors
  {
    if (msg.descriptors.size() == 0) {
      out << "descriptors: []";
    } else {
      out << "descriptors: [";
      size_t pending_items = msg.descriptors.size();
      for (auto item : msg.descriptors) {
        rosidl_generator_traits::value_to_yaml(item, out);
        if (--pending_items > 0) {
          out << ", ";
        }
      }
      out << "]";
    }
    out << ", ";
  }

  // member: num_keypoints
  {
    out << "num_keypoints: ";
    rosidl_generator_traits::value_to_yaml(msg.num_keypoints, out);
    out << ", ";
  }

  // member: publish_timestamp
  {
    out << "publish_timestamp: ";
    rosidl_generator_traits::value_to_yaml(msg.publish_timestamp, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const ImageFeatures & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: image_name
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "image_name: ";
    rosidl_generator_traits::value_to_yaml(msg.image_name, out);
    out << "\n";
  }

  // member: height
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "height: ";
    rosidl_generator_traits::value_to_yaml(msg.height, out);
    out << "\n";
  }

  // member: width
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "width: ";
    rosidl_generator_traits::value_to_yaml(msg.width, out);
    out << "\n";
  }

  // member: keypoints_x
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.keypoints_x.size() == 0) {
      out << "keypoints_x: []\n";
    } else {
      out << "keypoints_x:\n";
      for (auto item : msg.keypoints_x) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "- ";
        rosidl_generator_traits::value_to_yaml(item, out);
        out << "\n";
      }
    }
  }

  // member: keypoints_y
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.keypoints_y.size() == 0) {
      out << "keypoints_y: []\n";
    } else {
      out << "keypoints_y:\n";
      for (auto item : msg.keypoints_y) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "- ";
        rosidl_generator_traits::value_to_yaml(item, out);
        out << "\n";
      }
    }
  }

  // member: keypoints_size
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.keypoints_size.size() == 0) {
      out << "keypoints_size: []\n";
    } else {
      out << "keypoints_size:\n";
      for (auto item : msg.keypoints_size) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "- ";
        rosidl_generator_traits::value_to_yaml(item, out);
        out << "\n";
      }
    }
  }

  // member: keypoints_angle
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.keypoints_angle.size() == 0) {
      out << "keypoints_angle: []\n";
    } else {
      out << "keypoints_angle:\n";
      for (auto item : msg.keypoints_angle) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "- ";
        rosidl_generator_traits::value_to_yaml(item, out);
        out << "\n";
      }
    }
  }

  // member: descriptors
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.descriptors.size() == 0) {
      out << "descriptors: []\n";
    } else {
      out << "descriptors:\n";
      for (auto item : msg.descriptors) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "- ";
        rosidl_generator_traits::value_to_yaml(item, out);
        out << "\n";
      }
    }
  }

  // member: num_keypoints
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "num_keypoints: ";
    rosidl_generator_traits::value_to_yaml(msg.num_keypoints, out);
    out << "\n";
  }

  // member: publish_timestamp
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "publish_timestamp: ";
    rosidl_generator_traits::value_to_yaml(msg.publish_timestamp, out);
    out << "\n";
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const ImageFeatures & msg, bool use_flow_style = false)
{
  std::ostringstream out;
  if (use_flow_style) {
    to_flow_style_yaml(msg, out);
  } else {
    to_block_style_yaml(msg, out);
  }
  return out.str();
}

}  // namespace msg

}  // namespace sift_interfaces

namespace rosidl_generator_traits
{

[[deprecated("use sift_interfaces::msg::to_block_style_yaml() instead")]]
inline void to_yaml(
  const sift_interfaces::msg::ImageFeatures & msg,
  std::ostream & out, size_t indentation = 0)
{
  sift_interfaces::msg::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use sift_interfaces::msg::to_yaml() instead")]]
inline std::string to_yaml(const sift_interfaces::msg::ImageFeatures & msg)
{
  return sift_interfaces::msg::to_yaml(msg);
}

template<>
inline const char * data_type<sift_interfaces::msg::ImageFeatures>()
{
  return "sift_interfaces::msg::ImageFeatures";
}

template<>
inline const char * name<sift_interfaces::msg::ImageFeatures>()
{
  return "sift_interfaces/msg/ImageFeatures";
}

template<>
struct has_fixed_size<sift_interfaces::msg::ImageFeatures>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<sift_interfaces::msg::ImageFeatures>
  : std::integral_constant<bool, false> {};

template<>
struct is_message<sift_interfaces::msg::ImageFeatures>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // SIFT_INTERFACES__MSG__DETAIL__IMAGE_FEATURES__TRAITS_HPP_
