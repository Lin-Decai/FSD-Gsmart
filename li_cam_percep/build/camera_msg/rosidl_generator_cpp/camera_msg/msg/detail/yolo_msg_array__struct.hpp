// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from camera_msg:msg/YoloMsgArray.idl
// generated code does not contain a copyright notice

#ifndef CAMERA_MSG__MSG__DETAIL__YOLO_MSG_ARRAY__STRUCT_HPP_
#define CAMERA_MSG__MSG__DETAIL__YOLO_MSG_ARRAY__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


// Include directives for member types
// Member 'header'
#include "std_msgs/msg/detail/header__struct.hpp"
// Member 'cones'
#include "camera_msg/msg/detail/yolo_msg__struct.hpp"

#ifndef _WIN32
# define DEPRECATED__camera_msg__msg__YoloMsgArray __attribute__((deprecated))
#else
# define DEPRECATED__camera_msg__msg__YoloMsgArray __declspec(deprecated)
#endif

namespace camera_msg
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct YoloMsgArray_
{
  using Type = YoloMsgArray_<ContainerAllocator>;

  explicit YoloMsgArray_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : header(_init)
  {
    (void)_init;
  }

  explicit YoloMsgArray_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : header(_alloc, _init)
  {
    (void)_init;
  }

  // field types and members
  using _header_type =
    std_msgs::msg::Header_<ContainerAllocator>;
  _header_type header;
  using _cones_type =
    std::vector<camera_msg::msg::YoloMsg_<ContainerAllocator>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<camera_msg::msg::YoloMsg_<ContainerAllocator>>>;
  _cones_type cones;

  // setters for named parameter idiom
  Type & set__header(
    const std_msgs::msg::Header_<ContainerAllocator> & _arg)
  {
    this->header = _arg;
    return *this;
  }
  Type & set__cones(
    const std::vector<camera_msg::msg::YoloMsg_<ContainerAllocator>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<camera_msg::msg::YoloMsg_<ContainerAllocator>>> & _arg)
  {
    this->cones = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    camera_msg::msg::YoloMsgArray_<ContainerAllocator> *;
  using ConstRawPtr =
    const camera_msg::msg::YoloMsgArray_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<camera_msg::msg::YoloMsgArray_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<camera_msg::msg::YoloMsgArray_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      camera_msg::msg::YoloMsgArray_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<camera_msg::msg::YoloMsgArray_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      camera_msg::msg::YoloMsgArray_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<camera_msg::msg::YoloMsgArray_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<camera_msg::msg::YoloMsgArray_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<camera_msg::msg::YoloMsgArray_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__camera_msg__msg__YoloMsgArray
    std::shared_ptr<camera_msg::msg::YoloMsgArray_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__camera_msg__msg__YoloMsgArray
    std::shared_ptr<camera_msg::msg::YoloMsgArray_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const YoloMsgArray_ & other) const
  {
    if (this->header != other.header) {
      return false;
    }
    if (this->cones != other.cones) {
      return false;
    }
    return true;
  }
  bool operator!=(const YoloMsgArray_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct YoloMsgArray_

// alias to use template instance with default allocator
using YoloMsgArray =
  camera_msg::msg::YoloMsgArray_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace camera_msg

#endif  // CAMERA_MSG__MSG__DETAIL__YOLO_MSG_ARRAY__STRUCT_HPP_
