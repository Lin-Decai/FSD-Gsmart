// generated from rosidl_typesupport_fastrtps_c/resource/idl__type_support_c.cpp.em
// with input from camera_msg:msg/YoloMsg.idl
// generated code does not contain a copyright notice
#include "camera_msg/msg/detail/yolo_msg__rosidl_typesupport_fastrtps_c.h"


#include <cassert>
#include <limits>
#include <string>
#include "rosidl_typesupport_fastrtps_c/identifier.h"
#include "rosidl_typesupport_fastrtps_c/wstring_conversion.hpp"
#include "rosidl_typesupport_fastrtps_cpp/message_type_support.h"
#include "camera_msg/msg/rosidl_typesupport_fastrtps_c__visibility_control.h"
#include "camera_msg/msg/detail/yolo_msg__struct.h"
#include "camera_msg/msg/detail/yolo_msg__functions.h"
#include "fastcdr/Cdr.h"

#ifndef _WIN32
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wunused-parameter"
# ifdef __clang__
#  pragma clang diagnostic ignored "-Wdeprecated-register"
#  pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
# endif
#endif
#ifndef _WIN32
# pragma GCC diagnostic pop
#endif

// includes and forward declarations of message dependencies and their conversion functions

#if defined(__cplusplus)
extern "C"
{
#endif


// forward declare type support functions


using _YoloMsg__ros_msg_type = camera_msg__msg__YoloMsg;

static bool _YoloMsg__cdr_serialize(
  const void * untyped_ros_message,
  eprosima::fastcdr::Cdr & cdr)
{
  if (!untyped_ros_message) {
    fprintf(stderr, "ros message handle is null\n");
    return false;
  }
  const _YoloMsg__ros_msg_type * ros_message = static_cast<const _YoloMsg__ros_msg_type *>(untyped_ros_message);
  // Field name: xmin
  {
    cdr << ros_message->xmin;
  }

  // Field name: ymin
  {
    cdr << ros_message->ymin;
  }

  // Field name: xmax
  {
    cdr << ros_message->xmax;
  }

  // Field name: ymax
  {
    cdr << ros_message->ymax;
  }

  // Field name: label
  {
    cdr << ros_message->label;
  }

  // Field name: confidence
  {
    cdr << ros_message->confidence;
  }

  // Field name: zmin
  {
    cdr << ros_message->zmin;
  }

  // Field name: zmax
  {
    cdr << ros_message->zmax;
  }

  return true;
}

static bool _YoloMsg__cdr_deserialize(
  eprosima::fastcdr::Cdr & cdr,
  void * untyped_ros_message)
{
  if (!untyped_ros_message) {
    fprintf(stderr, "ros message handle is null\n");
    return false;
  }
  _YoloMsg__ros_msg_type * ros_message = static_cast<_YoloMsg__ros_msg_type *>(untyped_ros_message);
  // Field name: xmin
  {
    cdr >> ros_message->xmin;
  }

  // Field name: ymin
  {
    cdr >> ros_message->ymin;
  }

  // Field name: xmax
  {
    cdr >> ros_message->xmax;
  }

  // Field name: ymax
  {
    cdr >> ros_message->ymax;
  }

  // Field name: label
  {
    cdr >> ros_message->label;
  }

  // Field name: confidence
  {
    cdr >> ros_message->confidence;
  }

  // Field name: zmin
  {
    cdr >> ros_message->zmin;
  }

  // Field name: zmax
  {
    cdr >> ros_message->zmax;
  }

  return true;
}  // NOLINT(readability/fn_size)

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_camera_msg
size_t get_serialized_size_camera_msg__msg__YoloMsg(
  const void * untyped_ros_message,
  size_t current_alignment)
{
  const _YoloMsg__ros_msg_type * ros_message = static_cast<const _YoloMsg__ros_msg_type *>(untyped_ros_message);
  (void)ros_message;
  size_t initial_alignment = current_alignment;

  const size_t padding = 4;
  const size_t wchar_size = 4;
  (void)padding;
  (void)wchar_size;

  // field.name xmin
  {
    size_t item_size = sizeof(ros_message->xmin);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // field.name ymin
  {
    size_t item_size = sizeof(ros_message->ymin);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // field.name xmax
  {
    size_t item_size = sizeof(ros_message->xmax);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // field.name ymax
  {
    size_t item_size = sizeof(ros_message->ymax);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // field.name label
  {
    size_t item_size = sizeof(ros_message->label);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // field.name confidence
  {
    size_t item_size = sizeof(ros_message->confidence);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // field.name zmin
  {
    size_t item_size = sizeof(ros_message->zmin);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // field.name zmax
  {
    size_t item_size = sizeof(ros_message->zmax);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }

  return current_alignment - initial_alignment;
}

static uint32_t _YoloMsg__get_serialized_size(const void * untyped_ros_message)
{
  return static_cast<uint32_t>(
    get_serialized_size_camera_msg__msg__YoloMsg(
      untyped_ros_message, 0));
}

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_camera_msg
size_t max_serialized_size_camera_msg__msg__YoloMsg(
  bool & full_bounded,
  bool & is_plain,
  size_t current_alignment)
{
  size_t initial_alignment = current_alignment;

  const size_t padding = 4;
  const size_t wchar_size = 4;
  size_t last_member_size = 0;
  (void)last_member_size;
  (void)padding;
  (void)wchar_size;

  full_bounded = true;
  is_plain = true;

  // member: xmin
  {
    size_t array_size = 1;

    last_member_size = array_size * sizeof(uint32_t);
    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }
  // member: ymin
  {
    size_t array_size = 1;

    last_member_size = array_size * sizeof(uint32_t);
    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }
  // member: xmax
  {
    size_t array_size = 1;

    last_member_size = array_size * sizeof(uint32_t);
    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }
  // member: ymax
  {
    size_t array_size = 1;

    last_member_size = array_size * sizeof(uint32_t);
    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }
  // member: label
  {
    size_t array_size = 1;

    last_member_size = array_size * sizeof(uint32_t);
    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }
  // member: confidence
  {
    size_t array_size = 1;

    last_member_size = array_size * sizeof(uint32_t);
    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }
  // member: zmin
  {
    size_t array_size = 1;

    last_member_size = array_size * sizeof(uint32_t);
    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }
  // member: zmax
  {
    size_t array_size = 1;

    last_member_size = array_size * sizeof(uint32_t);
    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }

  size_t ret_val = current_alignment - initial_alignment;
  if (is_plain) {
    // All members are plain, and type is not empty.
    // We still need to check that the in-memory alignment
    // is the same as the CDR mandated alignment.
    using DataType = camera_msg__msg__YoloMsg;
    is_plain =
      (
      offsetof(DataType, zmax) +
      last_member_size
      ) == ret_val;
  }

  return ret_val;
}

static size_t _YoloMsg__max_serialized_size(char & bounds_info)
{
  bool full_bounded;
  bool is_plain;
  size_t ret_val;

  ret_val = max_serialized_size_camera_msg__msg__YoloMsg(
    full_bounded, is_plain, 0);

  bounds_info =
    is_plain ? ROSIDL_TYPESUPPORT_FASTRTPS_PLAIN_TYPE :
    full_bounded ? ROSIDL_TYPESUPPORT_FASTRTPS_BOUNDED_TYPE : ROSIDL_TYPESUPPORT_FASTRTPS_UNBOUNDED_TYPE;
  return ret_val;
}


static message_type_support_callbacks_t __callbacks_YoloMsg = {
  "camera_msg::msg",
  "YoloMsg",
  _YoloMsg__cdr_serialize,
  _YoloMsg__cdr_deserialize,
  _YoloMsg__get_serialized_size,
  _YoloMsg__max_serialized_size
};

static rosidl_message_type_support_t _YoloMsg__type_support = {
  rosidl_typesupport_fastrtps_c__identifier,
  &__callbacks_YoloMsg,
  get_message_typesupport_handle_function,
};

const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_c, camera_msg, msg, YoloMsg)() {
  return &_YoloMsg__type_support;
}

#if defined(__cplusplus)
}
#endif
