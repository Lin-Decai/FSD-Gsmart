// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from camera_msg:msg/YoloMsg.idl
// generated code does not contain a copyright notice

#ifndef CAMERA_MSG__MSG__DETAIL__YOLO_MSG__TRAITS_HPP_
#define CAMERA_MSG__MSG__DETAIL__YOLO_MSG__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "camera_msg/msg/detail/yolo_msg__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

namespace camera_msg
{

namespace msg
{

inline void to_flow_style_yaml(
  const YoloMsg & msg,
  std::ostream & out)
{
  out << "{";
  // member: xmin
  {
    out << "xmin: ";
    rosidl_generator_traits::value_to_yaml(msg.xmin, out);
    out << ", ";
  }

  // member: ymin
  {
    out << "ymin: ";
    rosidl_generator_traits::value_to_yaml(msg.ymin, out);
    out << ", ";
  }

  // member: xmax
  {
    out << "xmax: ";
    rosidl_generator_traits::value_to_yaml(msg.xmax, out);
    out << ", ";
  }

  // member: ymax
  {
    out << "ymax: ";
    rosidl_generator_traits::value_to_yaml(msg.ymax, out);
    out << ", ";
  }

  // member: label
  {
    out << "label: ";
    rosidl_generator_traits::value_to_yaml(msg.label, out);
    out << ", ";
  }

  // member: confidence
  {
    out << "confidence: ";
    rosidl_generator_traits::value_to_yaml(msg.confidence, out);
    out << ", ";
  }

  // member: zmin
  {
    out << "zmin: ";
    rosidl_generator_traits::value_to_yaml(msg.zmin, out);
    out << ", ";
  }

  // member: zmax
  {
    out << "zmax: ";
    rosidl_generator_traits::value_to_yaml(msg.zmax, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const YoloMsg & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: xmin
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "xmin: ";
    rosidl_generator_traits::value_to_yaml(msg.xmin, out);
    out << "\n";
  }

  // member: ymin
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "ymin: ";
    rosidl_generator_traits::value_to_yaml(msg.ymin, out);
    out << "\n";
  }

  // member: xmax
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "xmax: ";
    rosidl_generator_traits::value_to_yaml(msg.xmax, out);
    out << "\n";
  }

  // member: ymax
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "ymax: ";
    rosidl_generator_traits::value_to_yaml(msg.ymax, out);
    out << "\n";
  }

  // member: label
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "label: ";
    rosidl_generator_traits::value_to_yaml(msg.label, out);
    out << "\n";
  }

  // member: confidence
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "confidence: ";
    rosidl_generator_traits::value_to_yaml(msg.confidence, out);
    out << "\n";
  }

  // member: zmin
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "zmin: ";
    rosidl_generator_traits::value_to_yaml(msg.zmin, out);
    out << "\n";
  }

  // member: zmax
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "zmax: ";
    rosidl_generator_traits::value_to_yaml(msg.zmax, out);
    out << "\n";
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const YoloMsg & msg, bool use_flow_style = false)
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

}  // namespace camera_msg

namespace rosidl_generator_traits
{

[[deprecated("use camera_msg::msg::to_block_style_yaml() instead")]]
inline void to_yaml(
  const camera_msg::msg::YoloMsg & msg,
  std::ostream & out, size_t indentation = 0)
{
  camera_msg::msg::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use camera_msg::msg::to_yaml() instead")]]
inline std::string to_yaml(const camera_msg::msg::YoloMsg & msg)
{
  return camera_msg::msg::to_yaml(msg);
}

template<>
inline const char * data_type<camera_msg::msg::YoloMsg>()
{
  return "camera_msg::msg::YoloMsg";
}

template<>
inline const char * name<camera_msg::msg::YoloMsg>()
{
  return "camera_msg/msg/YoloMsg";
}

template<>
struct has_fixed_size<camera_msg::msg::YoloMsg>
  : std::integral_constant<bool, true> {};

template<>
struct has_bounded_size<camera_msg::msg::YoloMsg>
  : std::integral_constant<bool, true> {};

template<>
struct is_message<camera_msg::msg::YoloMsg>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // CAMERA_MSG__MSG__DETAIL__YOLO_MSG__TRAITS_HPP_
