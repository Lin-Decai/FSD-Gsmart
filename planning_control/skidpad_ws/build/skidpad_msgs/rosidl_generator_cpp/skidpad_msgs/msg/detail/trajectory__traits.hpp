// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from skidpad_msgs:msg/Trajectory.idl
// generated code does not contain a copyright notice

#ifndef SKIDPAD_MSGS__MSG__DETAIL__TRAJECTORY__TRAITS_HPP_
#define SKIDPAD_MSGS__MSG__DETAIL__TRAJECTORY__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "skidpad_msgs/msg/detail/trajectory__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

// Include directives for member types
// Member 'header'
#include "std_msgs/msg/detail/header__traits.hpp"
// Member 'trajectory'
#include "skidpad_msgs/msg/detail/trajectory_point__traits.hpp"
// Member 'remaining_distance'
#include "std_msgs/msg/detail/float64__traits.hpp"

namespace skidpad_msgs
{

namespace msg
{

inline void to_flow_style_yaml(
  const Trajectory & msg,
  std::ostream & out)
{
  out << "{";
  // member: header
  {
    out << "header: ";
    to_flow_style_yaml(msg.header, out);
    out << ", ";
  }

  // member: trajectory
  {
    if (msg.trajectory.size() == 0) {
      out << "trajectory: []";
    } else {
      out << "trajectory: [";
      size_t pending_items = msg.trajectory.size();
      for (auto item : msg.trajectory) {
        to_flow_style_yaml(item, out);
        if (--pending_items > 0) {
          out << ", ";
        }
      }
      out << "]";
    }
    out << ", ";
  }

  // member: remaining_distance
  {
    out << "remaining_distance: ";
    to_flow_style_yaml(msg.remaining_distance, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const Trajectory & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: header
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "header:\n";
    to_block_style_yaml(msg.header, out, indentation + 2);
  }

  // member: trajectory
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.trajectory.size() == 0) {
      out << "trajectory: []\n";
    } else {
      out << "trajectory:\n";
      for (auto item : msg.trajectory) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "-\n";
        to_block_style_yaml(item, out, indentation + 2);
      }
    }
  }

  // member: remaining_distance
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "remaining_distance:\n";
    to_block_style_yaml(msg.remaining_distance, out, indentation + 2);
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const Trajectory & msg, bool use_flow_style = false)
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
  const skidpad_msgs::msg::Trajectory & msg,
  std::ostream & out, size_t indentation = 0)
{
  skidpad_msgs::msg::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use skidpad_msgs::msg::to_yaml() instead")]]
inline std::string to_yaml(const skidpad_msgs::msg::Trajectory & msg)
{
  return skidpad_msgs::msg::to_yaml(msg);
}

template<>
inline const char * data_type<skidpad_msgs::msg::Trajectory>()
{
  return "skidpad_msgs::msg::Trajectory";
}

template<>
inline const char * name<skidpad_msgs::msg::Trajectory>()
{
  return "skidpad_msgs/msg/Trajectory";
}

template<>
struct has_fixed_size<skidpad_msgs::msg::Trajectory>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<skidpad_msgs::msg::Trajectory>
  : std::integral_constant<bool, false> {};

template<>
struct is_message<skidpad_msgs::msg::Trajectory>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // SKIDPAD_MSGS__MSG__DETAIL__TRAJECTORY__TRAITS_HPP_
