// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from skidpad_msgs:msg/TrajectoryPoint.idl
// generated code does not contain a copyright notice

#ifndef SKIDPAD_MSGS__MSG__DETAIL__TRAJECTORY_POINT__TRAITS_HPP_
#define SKIDPAD_MSGS__MSG__DETAIL__TRAJECTORY_POINT__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "skidpad_msgs/msg/detail/trajectory_point__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

// Include directives for member types
// Member 'pts'
#include "geometry_msgs/msg/detail/point__traits.hpp"
// Member 'yaw'
// Member 'curvature'
// Member 'velocity'
// Member 'r'
// Member 'acc'
#include "std_msgs/msg/detail/float64__traits.hpp"

namespace skidpad_msgs
{

namespace msg
{

inline void to_flow_style_yaml(
  const TrajectoryPoint & msg,
  std::ostream & out)
{
  out << "{";
  // member: pts
  {
    out << "pts: ";
    to_flow_style_yaml(msg.pts, out);
    out << ", ";
  }

  // member: yaw
  {
    out << "yaw: ";
    to_flow_style_yaml(msg.yaw, out);
    out << ", ";
  }

  // member: curvature
  {
    out << "curvature: ";
    to_flow_style_yaml(msg.curvature, out);
    out << ", ";
  }

  // member: velocity
  {
    out << "velocity: ";
    to_flow_style_yaml(msg.velocity, out);
    out << ", ";
  }

  // member: r
  {
    out << "r: ";
    to_flow_style_yaml(msg.r, out);
    out << ", ";
  }

  // member: acc
  {
    out << "acc: ";
    to_flow_style_yaml(msg.acc, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const TrajectoryPoint & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: pts
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "pts:\n";
    to_block_style_yaml(msg.pts, out, indentation + 2);
  }

  // member: yaw
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "yaw:\n";
    to_block_style_yaml(msg.yaw, out, indentation + 2);
  }

  // member: curvature
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "curvature:\n";
    to_block_style_yaml(msg.curvature, out, indentation + 2);
  }

  // member: velocity
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "velocity:\n";
    to_block_style_yaml(msg.velocity, out, indentation + 2);
  }

  // member: r
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "r:\n";
    to_block_style_yaml(msg.r, out, indentation + 2);
  }

  // member: acc
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "acc:\n";
    to_block_style_yaml(msg.acc, out, indentation + 2);
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const TrajectoryPoint & msg, bool use_flow_style = false)
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

}  // namespace skidpad_msgs

namespace rosidl_generator_traits
{

[[deprecated("use skidpad_msgs::msg::to_block_style_yaml() instead")]]
inline void to_yaml(
  const skidpad_msgs::msg::TrajectoryPoint & msg,
  std::ostream & out, size_t indentation = 0)
{
  skidpad_msgs::msg::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use skidpad_msgs::msg::to_yaml() instead")]]
inline std::string to_yaml(const skidpad_msgs::msg::TrajectoryPoint & msg)
{
  return skidpad_msgs::msg::to_yaml(msg);
}

template<>
inline const char * data_type<skidpad_msgs::msg::TrajectoryPoint>()
{
  return "skidpad_msgs::msg::TrajectoryPoint";
}

template<>
inline const char * name<skidpad_msgs::msg::TrajectoryPoint>()
{
  return "skidpad_msgs/msg/TrajectoryPoint";
}

template<>
struct has_fixed_size<skidpad_msgs::msg::TrajectoryPoint>
  : std::integral_constant<bool, has_fixed_size<geometry_msgs::msg::Point>::value && has_fixed_size<std_msgs::msg::Float64>::value> {};

template<>
struct has_bounded_size<skidpad_msgs::msg::TrajectoryPoint>
  : std::integral_constant<bool, has_bounded_size<geometry_msgs::msg::Point>::value && has_bounded_size<std_msgs::msg::Float64>::value> {};

template<>
struct is_message<skidpad_msgs::msg::TrajectoryPoint>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // SKIDPAD_MSGS__MSG__DETAIL__TRAJECTORY_POINT__TRAITS_HPP_
