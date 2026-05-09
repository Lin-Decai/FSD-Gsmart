// generated from rosidl_typesupport_introspection_c/resource/idl__type_support.c.em
// with input from skidpad_msgs:msg/CarStateDt.idl
// generated code does not contain a copyright notice

#include <stddef.h>
#include "skidpad_msgs/msg/detail/car_state_dt__rosidl_typesupport_introspection_c.h"
#include "skidpad_msgs/msg/rosidl_typesupport_introspection_c__visibility_control.h"
#include "rosidl_typesupport_introspection_c/field_types.h"
#include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/message_introspection.h"
#include "skidpad_msgs/msg/detail/car_state_dt__functions.h"
#include "skidpad_msgs/msg/detail/car_state_dt__struct.h"


// Include directives for member types
// Member `header`
#include "std_msgs/msg/header.h"
// Member `header`
#include "std_msgs/msg/detail/header__rosidl_typesupport_introspection_c.h"
// Member `car_state_dt`
// Member `car_state_a`
#include "geometry_msgs/msg/pose2_d.h"
// Member `car_state_dt`
// Member `car_state_a`
#include "geometry_msgs/msg/detail/pose2_d__rosidl_typesupport_introspection_c.h"

#ifdef __cplusplus
extern "C"
{
#endif

void skidpad_msgs__msg__CarStateDt__rosidl_typesupport_introspection_c__CarStateDt_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  skidpad_msgs__msg__CarStateDt__init(message_memory);
}

void skidpad_msgs__msg__CarStateDt__rosidl_typesupport_introspection_c__CarStateDt_fini_function(void * message_memory)
{
  skidpad_msgs__msg__CarStateDt__fini(message_memory);
}

static rosidl_typesupport_introspection_c__MessageMember skidpad_msgs__msg__CarStateDt__rosidl_typesupport_introspection_c__CarStateDt_message_member_array[3] = {
  {
    "header",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(skidpad_msgs__msg__CarStateDt, header),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "car_state_dt",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(skidpad_msgs__msg__CarStateDt, car_state_dt),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "car_state_a",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(skidpad_msgs__msg__CarStateDt, car_state_a),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers skidpad_msgs__msg__CarStateDt__rosidl_typesupport_introspection_c__CarStateDt_message_members = {
  "skidpad_msgs__msg",  // message namespace
  "CarStateDt",  // message name
  3,  // number of fields
  sizeof(skidpad_msgs__msg__CarStateDt),
  skidpad_msgs__msg__CarStateDt__rosidl_typesupport_introspection_c__CarStateDt_message_member_array,  // message members
  skidpad_msgs__msg__CarStateDt__rosidl_typesupport_introspection_c__CarStateDt_init_function,  // function to initialize message memory (memory has to be allocated)
  skidpad_msgs__msg__CarStateDt__rosidl_typesupport_introspection_c__CarStateDt_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t skidpad_msgs__msg__CarStateDt__rosidl_typesupport_introspection_c__CarStateDt_message_type_support_handle = {
  0,
  &skidpad_msgs__msg__CarStateDt__rosidl_typesupport_introspection_c__CarStateDt_message_members,
  get_message_typesupport_handle_function,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_skidpad_msgs
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, skidpad_msgs, msg, CarStateDt)() {
  skidpad_msgs__msg__CarStateDt__rosidl_typesupport_introspection_c__CarStateDt_message_member_array[0].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, std_msgs, msg, Header)();
  skidpad_msgs__msg__CarStateDt__rosidl_typesupport_introspection_c__CarStateDt_message_member_array[1].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, geometry_msgs, msg, Pose2D)();
  skidpad_msgs__msg__CarStateDt__rosidl_typesupport_introspection_c__CarStateDt_message_member_array[2].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, geometry_msgs, msg, Pose2D)();
  if (!skidpad_msgs__msg__CarStateDt__rosidl_typesupport_introspection_c__CarStateDt_message_type_support_handle.typesupport_identifier) {
    skidpad_msgs__msg__CarStateDt__rosidl_typesupport_introspection_c__CarStateDt_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &skidpad_msgs__msg__CarStateDt__rosidl_typesupport_introspection_c__CarStateDt_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif
