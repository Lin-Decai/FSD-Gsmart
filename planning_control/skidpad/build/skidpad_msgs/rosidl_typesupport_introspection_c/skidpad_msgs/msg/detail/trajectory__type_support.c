// generated from rosidl_typesupport_introspection_c/resource/idl__type_support.c.em
// with input from skidpad_msgs:msg/Trajectory.idl
// generated code does not contain a copyright notice

#include <stddef.h>
#include "skidpad_msgs/msg/detail/trajectory__rosidl_typesupport_introspection_c.h"
#include "skidpad_msgs/msg/rosidl_typesupport_introspection_c__visibility_control.h"
#include "rosidl_typesupport_introspection_c/field_types.h"
#include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/message_introspection.h"
#include "skidpad_msgs/msg/detail/trajectory__functions.h"
#include "skidpad_msgs/msg/detail/trajectory__struct.h"


// Include directives for member types
// Member `header`
#include "std_msgs/msg/header.h"
// Member `header`
#include "std_msgs/msg/detail/header__rosidl_typesupport_introspection_c.h"
// Member `trajectory`
#include "skidpad_msgs/msg/trajectory_point.h"
// Member `trajectory`
#include "skidpad_msgs/msg/detail/trajectory_point__rosidl_typesupport_introspection_c.h"
// Member `remaining_distance`
#include "std_msgs/msg/float64.h"
// Member `remaining_distance`
#include "std_msgs/msg/detail/float64__rosidl_typesupport_introspection_c.h"

#ifdef __cplusplus
extern "C"
{
#endif

void skidpad_msgs__msg__Trajectory__rosidl_typesupport_introspection_c__Trajectory_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  skidpad_msgs__msg__Trajectory__init(message_memory);
}

void skidpad_msgs__msg__Trajectory__rosidl_typesupport_introspection_c__Trajectory_fini_function(void * message_memory)
{
  skidpad_msgs__msg__Trajectory__fini(message_memory);
}

size_t skidpad_msgs__msg__Trajectory__rosidl_typesupport_introspection_c__size_function__Trajectory__trajectory(
  const void * untyped_member)
{
  const skidpad_msgs__msg__TrajectoryPoint__Sequence * member =
    (const skidpad_msgs__msg__TrajectoryPoint__Sequence *)(untyped_member);
  return member->size;
}

const void * skidpad_msgs__msg__Trajectory__rosidl_typesupport_introspection_c__get_const_function__Trajectory__trajectory(
  const void * untyped_member, size_t index)
{
  const skidpad_msgs__msg__TrajectoryPoint__Sequence * member =
    (const skidpad_msgs__msg__TrajectoryPoint__Sequence *)(untyped_member);
  return &member->data[index];
}

void * skidpad_msgs__msg__Trajectory__rosidl_typesupport_introspection_c__get_function__Trajectory__trajectory(
  void * untyped_member, size_t index)
{
  skidpad_msgs__msg__TrajectoryPoint__Sequence * member =
    (skidpad_msgs__msg__TrajectoryPoint__Sequence *)(untyped_member);
  return &member->data[index];
}

void skidpad_msgs__msg__Trajectory__rosidl_typesupport_introspection_c__fetch_function__Trajectory__trajectory(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const skidpad_msgs__msg__TrajectoryPoint * item =
    ((const skidpad_msgs__msg__TrajectoryPoint *)
    skidpad_msgs__msg__Trajectory__rosidl_typesupport_introspection_c__get_const_function__Trajectory__trajectory(untyped_member, index));
  skidpad_msgs__msg__TrajectoryPoint * value =
    (skidpad_msgs__msg__TrajectoryPoint *)(untyped_value);
  *value = *item;
}

void skidpad_msgs__msg__Trajectory__rosidl_typesupport_introspection_c__assign_function__Trajectory__trajectory(
  void * untyped_member, size_t index, const void * untyped_value)
{
  skidpad_msgs__msg__TrajectoryPoint * item =
    ((skidpad_msgs__msg__TrajectoryPoint *)
    skidpad_msgs__msg__Trajectory__rosidl_typesupport_introspection_c__get_function__Trajectory__trajectory(untyped_member, index));
  const skidpad_msgs__msg__TrajectoryPoint * value =
    (const skidpad_msgs__msg__TrajectoryPoint *)(untyped_value);
  *item = *value;
}

bool skidpad_msgs__msg__Trajectory__rosidl_typesupport_introspection_c__resize_function__Trajectory__trajectory(
  void * untyped_member, size_t size)
{
  skidpad_msgs__msg__TrajectoryPoint__Sequence * member =
    (skidpad_msgs__msg__TrajectoryPoint__Sequence *)(untyped_member);
  skidpad_msgs__msg__TrajectoryPoint__Sequence__fini(member);
  return skidpad_msgs__msg__TrajectoryPoint__Sequence__init(member, size);
}

static rosidl_typesupport_introspection_c__MessageMember skidpad_msgs__msg__Trajectory__rosidl_typesupport_introspection_c__Trajectory_message_member_array[3] = {
  {
    "header",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(skidpad_msgs__msg__Trajectory, header),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "trajectory",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(skidpad_msgs__msg__Trajectory, trajectory),  // bytes offset in struct
    NULL,  // default value
    skidpad_msgs__msg__Trajectory__rosidl_typesupport_introspection_c__size_function__Trajectory__trajectory,  // size() function pointer
    skidpad_msgs__msg__Trajectory__rosidl_typesupport_introspection_c__get_const_function__Trajectory__trajectory,  // get_const(index) function pointer
    skidpad_msgs__msg__Trajectory__rosidl_typesupport_introspection_c__get_function__Trajectory__trajectory,  // get(index) function pointer
    skidpad_msgs__msg__Trajectory__rosidl_typesupport_introspection_c__fetch_function__Trajectory__trajectory,  // fetch(index, &value) function pointer
    skidpad_msgs__msg__Trajectory__rosidl_typesupport_introspection_c__assign_function__Trajectory__trajectory,  // assign(index, value) function pointer
    skidpad_msgs__msg__Trajectory__rosidl_typesupport_introspection_c__resize_function__Trajectory__trajectory  // resize(index) function pointer
  },
  {
    "remaining_distance",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(skidpad_msgs__msg__Trajectory, remaining_distance),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers skidpad_msgs__msg__Trajectory__rosidl_typesupport_introspection_c__Trajectory_message_members = {
  "skidpad_msgs__msg",  // message namespace
  "Trajectory",  // message name
  3,  // number of fields
  sizeof(skidpad_msgs__msg__Trajectory),
  skidpad_msgs__msg__Trajectory__rosidl_typesupport_introspection_c__Trajectory_message_member_array,  // message members
  skidpad_msgs__msg__Trajectory__rosidl_typesupport_introspection_c__Trajectory_init_function,  // function to initialize message memory (memory has to be allocated)
  skidpad_msgs__msg__Trajectory__rosidl_typesupport_introspection_c__Trajectory_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t skidpad_msgs__msg__Trajectory__rosidl_typesupport_introspection_c__Trajectory_message_type_support_handle = {
  0,
  &skidpad_msgs__msg__Trajectory__rosidl_typesupport_introspection_c__Trajectory_message_members,
  get_message_typesupport_handle_function,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_skidpad_msgs
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, skidpad_msgs, msg, Trajectory)() {
  skidpad_msgs__msg__Trajectory__rosidl_typesupport_introspection_c__Trajectory_message_member_array[0].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, std_msgs, msg, Header)();
  skidpad_msgs__msg__Trajectory__rosidl_typesupport_introspection_c__Trajectory_message_member_array[1].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, skidpad_msgs, msg, TrajectoryPoint)();
  skidpad_msgs__msg__Trajectory__rosidl_typesupport_introspection_c__Trajectory_message_member_array[2].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, std_msgs, msg, Float64)();
  if (!skidpad_msgs__msg__Trajectory__rosidl_typesupport_introspection_c__Trajectory_message_type_support_handle.typesupport_identifier) {
    skidpad_msgs__msg__Trajectory__rosidl_typesupport_introspection_c__Trajectory_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &skidpad_msgs__msg__Trajectory__rosidl_typesupport_introspection_c__Trajectory_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif
