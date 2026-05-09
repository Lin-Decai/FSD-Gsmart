// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from camera_msg:msg/YoloMsg.idl
// generated code does not contain a copyright notice

#ifndef CAMERA_MSG__MSG__DETAIL__YOLO_MSG__STRUCT_HPP_
#define CAMERA_MSG__MSG__DETAIL__YOLO_MSG__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


#ifndef _WIN32
# define DEPRECATED__camera_msg__msg__YoloMsg __attribute__((deprecated))
#else
# define DEPRECATED__camera_msg__msg__YoloMsg __declspec(deprecated)
#endif

namespace camera_msg
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct YoloMsg_
{
  using Type = YoloMsg_<ContainerAllocator>;

  explicit YoloMsg_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->xmin = 0ul;
      this->ymin = 0ul;
      this->xmax = 0ul;
      this->ymax = 0ul;
      this->label = 0.0f;
      this->confidence = 0.0f;
      this->zmin = 0.0f;
      this->zmax = 0.0f;
    }
  }

  explicit YoloMsg_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_alloc;
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->xmin = 0ul;
      this->ymin = 0ul;
      this->xmax = 0ul;
      this->ymax = 0ul;
      this->label = 0.0f;
      this->confidence = 0.0f;
      this->zmin = 0.0f;
      this->zmax = 0.0f;
    }
  }

  // field types and members
  using _xmin_type =
    uint32_t;
  _xmin_type xmin;
  using _ymin_type =
    uint32_t;
  _ymin_type ymin;
  using _xmax_type =
    uint32_t;
  _xmax_type xmax;
  using _ymax_type =
    uint32_t;
  _ymax_type ymax;
  using _label_type =
    float;
  _label_type label;
  using _confidence_type =
    float;
  _confidence_type confidence;
  using _zmin_type =
    float;
  _zmin_type zmin;
  using _zmax_type =
    float;
  _zmax_type zmax;

  // setters for named parameter idiom
  Type & set__xmin(
    const uint32_t & _arg)
  {
    this->xmin = _arg;
    return *this;
  }
  Type & set__ymin(
    const uint32_t & _arg)
  {
    this->ymin = _arg;
    return *this;
  }
  Type & set__xmax(
    const uint32_t & _arg)
  {
    this->xmax = _arg;
    return *this;
  }
  Type & set__ymax(
    const uint32_t & _arg)
  {
    this->ymax = _arg;
    return *this;
  }
  Type & set__label(
    const float & _arg)
  {
    this->label = _arg;
    return *this;
  }
  Type & set__confidence(
    const float & _arg)
  {
    this->confidence = _arg;
    return *this;
  }
  Type & set__zmin(
    const float & _arg)
  {
    this->zmin = _arg;
    return *this;
  }
  Type & set__zmax(
    const float & _arg)
  {
    this->zmax = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    camera_msg::msg::YoloMsg_<ContainerAllocator> *;
  using ConstRawPtr =
    const camera_msg::msg::YoloMsg_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<camera_msg::msg::YoloMsg_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<camera_msg::msg::YoloMsg_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      camera_msg::msg::YoloMsg_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<camera_msg::msg::YoloMsg_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      camera_msg::msg::YoloMsg_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<camera_msg::msg::YoloMsg_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<camera_msg::msg::YoloMsg_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<camera_msg::msg::YoloMsg_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__camera_msg__msg__YoloMsg
    std::shared_ptr<camera_msg::msg::YoloMsg_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__camera_msg__msg__YoloMsg
    std::shared_ptr<camera_msg::msg::YoloMsg_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const YoloMsg_ & other) const
  {
    if (this->xmin != other.xmin) {
      return false;
    }
    if (this->ymin != other.ymin) {
      return false;
    }
    if (this->xmax != other.xmax) {
      return false;
    }
    if (this->ymax != other.ymax) {
      return false;
    }
    if (this->label != other.label) {
      return false;
    }
    if (this->confidence != other.confidence) {
      return false;
    }
    if (this->zmin != other.zmin) {
      return false;
    }
    if (this->zmax != other.zmax) {
      return false;
    }
    return true;
  }
  bool operator!=(const YoloMsg_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct YoloMsg_

// alias to use template instance with default allocator
using YoloMsg =
  camera_msg::msg::YoloMsg_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace camera_msg

#endif  // CAMERA_MSG__MSG__DETAIL__YOLO_MSG__STRUCT_HPP_
