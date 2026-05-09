// generated from rosidl_typesupport_introspection_c/resource/idl__type_support.c.em
// with input from camera_msg:msg/YoloMsgArray.idl
// generated code does not contain a copyright notice

#include <stddef.h>
#include "camera_msg/msg/detail/yolo_msg_array__rosidl_typesupport_introspection_c.h"
#include "camera_msg/msg/rosidl_typesupport_introspection_c__visibility_control.h"
#include "rosidl_typesupport_introspection_c/field_types.h"
#include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/message_introspection.h"
#include "camera_msg/msg/detail/yolo_msg_array__functions.h"
#include "camera_msg/msg/detail/yolo_msg_array__struct.h"


// Include directives for member types
// Member `header`
#include "std_msgs/msg/header.h"
// Member `header`
#include "std_msgs/msg/detail/header__rosidl_typesupport_introspection_c.h"
// Member `cones`
#include "camera_msg/msg/yolo_msg.h"
// Member `cones`
#include "camera_msg/msg/detail/yolo_msg__rosidl_typesupport_introspection_c.h"

#ifdef __cplusplus
extern "C"
{
#endif

void camera_msg__msg__YoloMsgArray__rosidl_typesupport_introspection_c__YoloMsgArray_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  camera_msg__msg__YoloMsgArray__init(message_memory);
}

void camera_msg__msg__YoloMsgArray__rosidl_typesupport_introspection_c__YoloMsgArray_fini_function(void * message_memory)
{
  camera_msg__msg__YoloMsgArray__fini(message_memory);
}

size_t camera_msg__msg__YoloMsgArray__rosidl_typesupport_introspection_c__size_function__YoloMsgArray__cones(
  const void * untyped_member)
{
  const camera_msg__msg__YoloMsg__Sequence * member =
    (const camera_msg__msg__YoloMsg__Sequence *)(untyped_member);
  return member->size;
}

const void * camera_msg__msg__YoloMsgArray__rosidl_typesupport_introspection_c__get_const_function__YoloMsgArray__cones(
  const void * untyped_member, size_t index)
{
  const camera_msg__msg__YoloMsg__Sequence * member =
    (const camera_msg__msg__YoloMsg__Sequence *)(untyped_member);
  return &member->data[index];
}

void * camera_msg__msg__YoloMsgArray__rosidl_typesupport_introspection_c__get_function__YoloMsgArray__cones(
  void * untyped_member, size_t index)
{
  camera_msg__msg__YoloMsg__Sequence * member =
    (camera_msg__msg__YoloMsg__Sequence *)(untyped_member);
  return &member->data[index];
}

void camera_msg__msg__YoloMsgArray__rosidl_typesupport_introspection_c__fetch_function__YoloMsgArray__cones(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const camera_msg__msg__YoloMsg * item =
    ((const camera_msg__msg__YoloMsg *)
    camera_msg__msg__YoloMsgArray__rosidl_typesupport_introspection_c__get_const_function__YoloMsgArray__cones(untyped_member, index));
  camera_msg__msg__YoloMsg * value =
    (camera_msg__msg__YoloMsg *)(untyped_value);
  *value = *item;
}

void camera_msg__msg__YoloMsgArray__rosidl_typesupport_introspection_c__assign_function__YoloMsgArray__cones(
  void * untyped_member, size_t index, const void * untyped_value)
{
  camera_msg__msg__YoloMsg * item =
    ((camera_msg__msg__YoloMsg *)
    camera_msg__msg__YoloMsgArray__rosidl_typesupport_introspection_c__get_function__YoloMsgArray__cones(untyped_member, index));
  const camera_msg__msg__YoloMsg * value =
    (const camera_msg__msg__YoloMsg *)(untyped_value);
  *item = *value;
}

bool camera_msg__msg__YoloMsgArray__rosidl_typesupport_introspection_c__resize_function__YoloMsgArray__cones(
  void * untyped_member, size_t size)
{
  camera_msg__msg__YoloMsg__Sequence * member =
    (camera_msg__msg__YoloMsg__Sequence *)(untyped_member);
  camera_msg__msg__YoloMsg__Sequence__fini(member);
  return camera_msg__msg__YoloMsg__Sequence__init(member, size);
}

static rosidl_typesupport_introspection_c__MessageMember camera_msg__msg__YoloMsgArray__rosidl_typesupport_introspection_c__YoloMsgArray_message_member_array[2] = {
  {
    "header",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(camera_msg__msg__YoloMsgArray, header),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "cones",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(camera_msg__msg__YoloMsgArray, cones),  // bytes offset in struct
    NULL,  // default value
    camera_msg__msg__YoloMsgArray__rosidl_typesupport_introspection_c__size_function__YoloMsgArray__cones,  // size() function pointer
    camera_msg__msg__YoloMsgArray__rosidl_typesupport_introspection_c__get_const_function__YoloMsgArray__cones,  // get_const(index) function pointer
    camera_msg__msg__YoloMsgArray__rosidl_typesupport_introspection_c__get_function__YoloMsgArray__cones,  // get(index) function pointer
    camera_msg__msg__YoloMsgArray__rosidl_typesupport_introspection_c__fetch_function__YoloMsgArray__cones,  // fetch(index, &value) function pointer
    camera_msg__msg__YoloMsgArray__rosidl_typesupport_introspection_c__assign_function__YoloMsgArray__cones,  // assign(index, value) function pointer
    camera_msg__msg__YoloMsgArray__rosidl_typesupport_introspection_c__resize_function__YoloMsgArray__cones  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers camera_msg__msg__YoloMsgArray__rosidl_typesupport_introspection_c__YoloMsgArray_message_members = {
  "camera_msg__msg",  // message namespace
  "YoloMsgArray",  // message name
  2,  // number of fields
  sizeof(camera_msg__msg__YoloMsgArray),
  camera_msg__msg__YoloMsgArray__rosidl_typesupport_introspection_c__YoloMsgArray_message_member_array,  // message members
  camera_msg__msg__YoloMsgArray__rosidl_typesupport_introspection_c__YoloMsgArray_init_function,  // function to initialize message memory (memory has to be allocated)
  camera_msg__msg__YoloMsgArray__rosidl_typesupport_introspection_c__YoloMsgArray_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t camera_msg__msg__YoloMsgArray__rosidl_typesupport_introspection_c__YoloMsgArray_message_type_support_handle = {
  0,
  &camera_msg__msg__YoloMsgArray__rosidl_typesupport_introspection_c__YoloMsgArray_message_members,
  get_message_typesupport_handle_function,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_camera_msg
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, camera_msg, msg, YoloMsgArray)() {
  camera_msg__msg__YoloMsgArray__rosidl_typesupport_introspection_c__YoloMsgArray_message_member_array[0].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, std_msgs, msg, Header)();
  camera_msg__msg__YoloMsgArray__rosidl_typesupport_introspection_c__YoloMsgArray_message_member_array[1].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, camera_msg, msg, YoloMsg)();
  if (!camera_msg__msg__YoloMsgArray__rosidl_typesupport_introspection_c__YoloMsgArray_message_type_support_handle.typesupport_identifier) {
    camera_msg__msg__YoloMsgArray__rosidl_typesupport_introspection_c__YoloMsgArray_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &camera_msg__msg__YoloMsgArray__rosidl_typesupport_introspection_c__YoloMsgArray_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif
