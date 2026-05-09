// generated from rosidl_typesupport_introspection_cpp/resource/idl__type_support.cpp.em
// with input from camera_msg:msg/YoloMsgArray.idl
// generated code does not contain a copyright notice

#include "array"
#include "cstddef"
#include "string"
#include "vector"
#include "rosidl_runtime_c/message_type_support_struct.h"
#include "rosidl_typesupport_cpp/message_type_support.hpp"
#include "rosidl_typesupport_interface/macros.h"
#include "camera_msg/msg/detail/yolo_msg_array__struct.hpp"
#include "rosidl_typesupport_introspection_cpp/field_types.hpp"
#include "rosidl_typesupport_introspection_cpp/identifier.hpp"
#include "rosidl_typesupport_introspection_cpp/message_introspection.hpp"
#include "rosidl_typesupport_introspection_cpp/message_type_support_decl.hpp"
#include "rosidl_typesupport_introspection_cpp/visibility_control.h"

namespace camera_msg
{

namespace msg
{

namespace rosidl_typesupport_introspection_cpp
{

void YoloMsgArray_init_function(
  void * message_memory, rosidl_runtime_cpp::MessageInitialization _init)
{
  new (message_memory) camera_msg::msg::YoloMsgArray(_init);
}

void YoloMsgArray_fini_function(void * message_memory)
{
  auto typed_message = static_cast<camera_msg::msg::YoloMsgArray *>(message_memory);
  typed_message->~YoloMsgArray();
}

size_t size_function__YoloMsgArray__cones(const void * untyped_member)
{
  const auto * member = reinterpret_cast<const std::vector<camera_msg::msg::YoloMsg> *>(untyped_member);
  return member->size();
}

const void * get_const_function__YoloMsgArray__cones(const void * untyped_member, size_t index)
{
  const auto & member =
    *reinterpret_cast<const std::vector<camera_msg::msg::YoloMsg> *>(untyped_member);
  return &member[index];
}

void * get_function__YoloMsgArray__cones(void * untyped_member, size_t index)
{
  auto & member =
    *reinterpret_cast<std::vector<camera_msg::msg::YoloMsg> *>(untyped_member);
  return &member[index];
}

void fetch_function__YoloMsgArray__cones(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const auto & item = *reinterpret_cast<const camera_msg::msg::YoloMsg *>(
    get_const_function__YoloMsgArray__cones(untyped_member, index));
  auto & value = *reinterpret_cast<camera_msg::msg::YoloMsg *>(untyped_value);
  value = item;
}

void assign_function__YoloMsgArray__cones(
  void * untyped_member, size_t index, const void * untyped_value)
{
  auto & item = *reinterpret_cast<camera_msg::msg::YoloMsg *>(
    get_function__YoloMsgArray__cones(untyped_member, index));
  const auto & value = *reinterpret_cast<const camera_msg::msg::YoloMsg *>(untyped_value);
  item = value;
}

void resize_function__YoloMsgArray__cones(void * untyped_member, size_t size)
{
  auto * member =
    reinterpret_cast<std::vector<camera_msg::msg::YoloMsg> *>(untyped_member);
  member->resize(size);
}

static const ::rosidl_typesupport_introspection_cpp::MessageMember YoloMsgArray_message_member_array[2] = {
  {
    "header",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<std_msgs::msg::Header>(),  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(camera_msg::msg::YoloMsgArray, header),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "cones",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<camera_msg::msg::YoloMsg>(),  // members of sub message
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(camera_msg::msg::YoloMsgArray, cones),  // bytes offset in struct
    nullptr,  // default value
    size_function__YoloMsgArray__cones,  // size() function pointer
    get_const_function__YoloMsgArray__cones,  // get_const(index) function pointer
    get_function__YoloMsgArray__cones,  // get(index) function pointer
    fetch_function__YoloMsgArray__cones,  // fetch(index, &value) function pointer
    assign_function__YoloMsgArray__cones,  // assign(index, value) function pointer
    resize_function__YoloMsgArray__cones  // resize(index) function pointer
  }
};

static const ::rosidl_typesupport_introspection_cpp::MessageMembers YoloMsgArray_message_members = {
  "camera_msg::msg",  // message namespace
  "YoloMsgArray",  // message name
  2,  // number of fields
  sizeof(camera_msg::msg::YoloMsgArray),
  YoloMsgArray_message_member_array,  // message members
  YoloMsgArray_init_function,  // function to initialize message memory (memory has to be allocated)
  YoloMsgArray_fini_function  // function to terminate message instance (will not free memory)
};

static const rosidl_message_type_support_t YoloMsgArray_message_type_support_handle = {
  ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  &YoloMsgArray_message_members,
  get_message_typesupport_handle_function,
};

}  // namespace rosidl_typesupport_introspection_cpp

}  // namespace msg

}  // namespace camera_msg


namespace rosidl_typesupport_introspection_cpp
{

template<>
ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
get_message_type_support_handle<camera_msg::msg::YoloMsgArray>()
{
  return &::camera_msg::msg::rosidl_typesupport_introspection_cpp::YoloMsgArray_message_type_support_handle;
}

}  // namespace rosidl_typesupport_introspection_cpp

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, camera_msg, msg, YoloMsgArray)() {
  return &::camera_msg::msg::rosidl_typesupport_introspection_cpp::YoloMsgArray_message_type_support_handle;
}

#ifdef __cplusplus
}
#endif
