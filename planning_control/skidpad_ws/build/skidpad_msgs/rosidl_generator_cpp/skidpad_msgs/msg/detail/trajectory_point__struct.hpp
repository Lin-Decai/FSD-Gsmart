// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from skidpad_msgs:msg/TrajectoryPoint.idl
// generated code does not contain a copyright notice

#ifndef SKIDPAD_MSGS__MSG__DETAIL__TRAJECTORY_POINT__STRUCT_HPP_
#define SKIDPAD_MSGS__MSG__DETAIL__TRAJECTORY_POINT__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


// Include directives for member types
// Member 'pts'
#include "geometry_msgs/msg/detail/point__struct.hpp"
// Member 'yaw'
// Member 'curvature'
// Member 'velocity'
// Member 'r'
// Member 'acc'
#include "std_msgs/msg/detail/float64__struct.hpp"

#ifndef _WIN32
# define DEPRECATED__skidpad_msgs__msg__TrajectoryPoint __attribute__((deprecated))
#else
# define DEPRECATED__skidpad_msgs__msg__TrajectoryPoint __declspec(deprecated)
#endif

namespace skidpad_msgs
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct TrajectoryPoint_
{
  using Type = TrajectoryPoint_<ContainerAllocator>;

  explicit TrajectoryPoint_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : pts(_init),
    yaw(_init),
    curvature(_init),
    velocity(_init),
    r(_init),
    acc(_init)
  {
    (void)_init;
  }

  explicit TrajectoryPoint_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : pts(_alloc, _init),
    yaw(_alloc, _init),
    curvature(_alloc, _init),
    velocity(_alloc, _init),
    r(_alloc, _init),
    acc(_alloc, _init)
  {
    (void)_init;
  }

  // field types and members
  using _pts_type =
    geometry_msgs::msg::Point_<ContainerAllocator>;
  _pts_type pts;
  using _yaw_type =
    std_msgs::msg::Float64_<ContainerAllocator>;
  _yaw_type yaw;
  using _curvature_type =
    std_msgs::msg::Float64_<ContainerAllocator>;
  _curvature_type curvature;
  using _velocity_type =
    std_msgs::msg::Float64_<ContainerAllocator>;
  _velocity_type velocity;
  using _r_type =
    std_msgs::msg::Float64_<ContainerAllocator>;
  _r_type r;
  using _acc_type =
    std_msgs::msg::Float64_<ContainerAllocator>;
  _acc_type acc;

  // setters for named parameter idiom
  Type & set__pts(
    const geometry_msgs::msg::Point_<ContainerAllocator> & _arg)
  {
    this->pts = _arg;
    return *this;
  }
  Type & set__yaw(
    const std_msgs::msg::Float64_<ContainerAllocator> & _arg)
  {
    this->yaw = _arg;
    return *this;
  }
  Type & set__curvature(
    const std_msgs::msg::Float64_<ContainerAllocator> & _arg)
  {
    this->curvature = _arg;
    return *this;
  }
  Type & set__velocity(
    const std_msgs::msg::Float64_<ContainerAllocator> & _arg)
  {
    this->velocity = _arg;
    return *this;
  }
  Type & set__r(
    const std_msgs::msg::Float64_<ContainerAllocator> & _arg)
  {
    this->r = _arg;
    return *this;
  }
  Type & set__acc(
    const std_msgs::msg::Float64_<ContainerAllocator> & _arg)
  {
    this->acc = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    skidpad_msgs::msg::TrajectoryPoint_<ContainerAllocator> *;
  using ConstRawPtr =
    const skidpad_msgs::msg::TrajectoryPoint_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<skidpad_msgs::msg::TrajectoryPoint_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<skidpad_msgs::msg::TrajectoryPoint_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      skidpad_msgs::msg::TrajectoryPoint_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<skidpad_msgs::msg::TrajectoryPoint_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      skidpad_msgs::msg::TrajectoryPoint_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<skidpad_msgs::msg::TrajectoryPoint_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<skidpad_msgs::msg::TrajectoryPoint_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<skidpad_msgs::msg::TrajectoryPoint_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__skidpad_msgs__msg__TrajectoryPoint
    std::shared_ptr<skidpad_msgs::msg::TrajectoryPoint_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__skidpad_msgs__msg__TrajectoryPoint
    std::shared_ptr<skidpad_msgs::msg::TrajectoryPoint_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const TrajectoryPoint_ & other) const
  {
    if (this->pts != other.pts) {
      return false;
    }
    if (this->yaw != other.yaw) {
      return false;
    }
    if (this->curvature != other.curvature) {
      return false;
    }
    if (this->velocity != other.velocity) {
      return false;
    }
    if (this->r != other.r) {
      return false;
    }
    if (this->acc != other.acc) {
      return false;
    }
    return true;
  }
  bool operator!=(const TrajectoryPoint_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct TrajectoryPoint_

// alias to use template instance with default allocator
using TrajectoryPoint =
  skidpad_msgs::msg::TrajectoryPoint_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace skidpad_msgs

#endif  // SKIDPAD_MSGS__MSG__DETAIL__TRAJECTORY_POINT__STRUCT_HPP_
