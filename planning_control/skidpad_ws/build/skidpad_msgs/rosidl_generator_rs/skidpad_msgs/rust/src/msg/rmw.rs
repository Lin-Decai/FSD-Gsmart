#[cfg(feature = "serde")]
use serde::{Deserialize, Serialize};


#[link(name = "skidpad_msgs__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_message_type_support_handle__skidpad_msgs__msg__CarState() -> *const std::ffi::c_void;
}

#[link(name = "skidpad_msgs__rosidl_generator_c")]
extern "C" {
    fn skidpad_msgs__msg__CarState__init(msg: *mut CarState) -> bool;
    fn skidpad_msgs__msg__CarState__Sequence__init(seq: *mut rosidl_runtime_rs::Sequence<CarState>, size: usize) -> bool;
    fn skidpad_msgs__msg__CarState__Sequence__fini(seq: *mut rosidl_runtime_rs::Sequence<CarState>);
    fn skidpad_msgs__msg__CarState__Sequence__copy(in_seq: &rosidl_runtime_rs::Sequence<CarState>, out_seq: *mut rosidl_runtime_rs::Sequence<CarState>) -> bool;
}

// Corresponds to skidpad_msgs__msg__CarState
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]

/// 车辆状态主消息

#[repr(C)]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct CarState {

    // This member is not documented.
    #[allow(missing_docs)]
    pub header: std_msgs::msg::rmw::Header,

    /// 位置和姿态角 (x, y, theta)
    pub car_state: geometry_msgs::msg::rmw::Pose2D,

    /// 一阶导数信息 (速度、加速度)
    pub car_state_dt: super::super::msg::rmw::CarStateDt,

}



impl Default for CarState {
  fn default() -> Self {
    unsafe {
      let mut msg = std::mem::zeroed();
      if !skidpad_msgs__msg__CarState__init(&mut msg as *mut _) {
        panic!("Call to skidpad_msgs__msg__CarState__init() failed");
      }
      msg
    }
  }
}

impl rosidl_runtime_rs::SequenceAlloc for CarState {
  fn sequence_init(seq: &mut rosidl_runtime_rs::Sequence<Self>, size: usize) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { skidpad_msgs__msg__CarState__Sequence__init(seq as *mut _, size) }
  }
  fn sequence_fini(seq: &mut rosidl_runtime_rs::Sequence<Self>) {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { skidpad_msgs__msg__CarState__Sequence__fini(seq as *mut _) }
  }
  fn sequence_copy(in_seq: &rosidl_runtime_rs::Sequence<Self>, out_seq: &mut rosidl_runtime_rs::Sequence<Self>) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { skidpad_msgs__msg__CarState__Sequence__copy(in_seq, out_seq as *mut _) }
  }
}

impl rosidl_runtime_rs::Message for CarState {
  type RmwMsg = Self;
  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> { msg_cow }
  fn from_rmw_message(msg: Self::RmwMsg) -> Self { msg }
}

impl rosidl_runtime_rs::RmwMessage for CarState where Self: Sized {
  const TYPE_NAME: &'static str = "skidpad_msgs/msg/CarState";
  fn get_type_support() -> *const std::ffi::c_void {
    // SAFETY: No preconditions for this function.
    unsafe { rosidl_typesupport_c__get_message_type_support_handle__skidpad_msgs__msg__CarState() }
  }
}


#[link(name = "skidpad_msgs__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_message_type_support_handle__skidpad_msgs__msg__CarStateDt() -> *const std::ffi::c_void;
}

#[link(name = "skidpad_msgs__rosidl_generator_c")]
extern "C" {
    fn skidpad_msgs__msg__CarStateDt__init(msg: *mut CarStateDt) -> bool;
    fn skidpad_msgs__msg__CarStateDt__Sequence__init(seq: *mut rosidl_runtime_rs::Sequence<CarStateDt>, size: usize) -> bool;
    fn skidpad_msgs__msg__CarStateDt__Sequence__fini(seq: *mut rosidl_runtime_rs::Sequence<CarStateDt>);
    fn skidpad_msgs__msg__CarStateDt__Sequence__copy(in_seq: &rosidl_runtime_rs::Sequence<CarStateDt>, out_seq: *mut rosidl_runtime_rs::Sequence<CarStateDt>) -> bool;
}

// Corresponds to skidpad_msgs__msg__CarStateDt
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]

/// 车辆状态的时间导数

#[repr(C)]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct CarStateDt {

    // This member is not documented.
    #[allow(missing_docs)]
    pub header: std_msgs::msg::rmw::Header,

    /// 速度 (x, y, theta)
    pub car_state_dt: geometry_msgs::msg::rmw::Pose2D,

    /// 加速度 (x, y, theta)
    pub car_state_a: geometry_msgs::msg::rmw::Pose2D,

}



impl Default for CarStateDt {
  fn default() -> Self {
    unsafe {
      let mut msg = std::mem::zeroed();
      if !skidpad_msgs__msg__CarStateDt__init(&mut msg as *mut _) {
        panic!("Call to skidpad_msgs__msg__CarStateDt__init() failed");
      }
      msg
    }
  }
}

impl rosidl_runtime_rs::SequenceAlloc for CarStateDt {
  fn sequence_init(seq: &mut rosidl_runtime_rs::Sequence<Self>, size: usize) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { skidpad_msgs__msg__CarStateDt__Sequence__init(seq as *mut _, size) }
  }
  fn sequence_fini(seq: &mut rosidl_runtime_rs::Sequence<Self>) {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { skidpad_msgs__msg__CarStateDt__Sequence__fini(seq as *mut _) }
  }
  fn sequence_copy(in_seq: &rosidl_runtime_rs::Sequence<Self>, out_seq: &mut rosidl_runtime_rs::Sequence<Self>) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { skidpad_msgs__msg__CarStateDt__Sequence__copy(in_seq, out_seq as *mut _) }
  }
}

impl rosidl_runtime_rs::Message for CarStateDt {
  type RmwMsg = Self;
  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> { msg_cow }
  fn from_rmw_message(msg: Self::RmwMsg) -> Self { msg }
}

impl rosidl_runtime_rs::RmwMessage for CarStateDt where Self: Sized {
  const TYPE_NAME: &'static str = "skidpad_msgs/msg/CarStateDt";
  fn get_type_support() -> *const std::ffi::c_void {
    // SAFETY: No preconditions for this function.
    unsafe { rosidl_typesupport_c__get_message_type_support_handle__skidpad_msgs__msg__CarStateDt() }
  }
}


#[link(name = "skidpad_msgs__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_message_type_support_handle__skidpad_msgs__msg__ControlCommand() -> *const std::ffi::c_void;
}

#[link(name = "skidpad_msgs__rosidl_generator_c")]
extern "C" {
    fn skidpad_msgs__msg__ControlCommand__init(msg: *mut ControlCommand) -> bool;
    fn skidpad_msgs__msg__ControlCommand__Sequence__init(seq: *mut rosidl_runtime_rs::Sequence<ControlCommand>, size: usize) -> bool;
    fn skidpad_msgs__msg__ControlCommand__Sequence__fini(seq: *mut rosidl_runtime_rs::Sequence<ControlCommand>);
    fn skidpad_msgs__msg__ControlCommand__Sequence__copy(in_seq: &rosidl_runtime_rs::Sequence<ControlCommand>, out_seq: *mut rosidl_runtime_rs::Sequence<ControlCommand>) -> bool;
}

// Corresponds to skidpad_msgs__msg__ControlCommand
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]

/// 控制命令

#[repr(C)]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct ControlCommand {

    // This member is not documented.
    #[allow(missing_docs)]
    pub header: std_msgs::msg::rmw::Header,

    /// 油门值，范围[-1, 1]，正为加速，负为制动
    pub throttle: std_msgs::msg::rmw::Float32,

    /// 转向角，范围[-1, 1]，左负右正
    pub steering_angle: std_msgs::msg::rmw::Float32,

}



impl Default for ControlCommand {
  fn default() -> Self {
    unsafe {
      let mut msg = std::mem::zeroed();
      if !skidpad_msgs__msg__ControlCommand__init(&mut msg as *mut _) {
        panic!("Call to skidpad_msgs__msg__ControlCommand__init() failed");
      }
      msg
    }
  }
}

impl rosidl_runtime_rs::SequenceAlloc for ControlCommand {
  fn sequence_init(seq: &mut rosidl_runtime_rs::Sequence<Self>, size: usize) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { skidpad_msgs__msg__ControlCommand__Sequence__init(seq as *mut _, size) }
  }
  fn sequence_fini(seq: &mut rosidl_runtime_rs::Sequence<Self>) {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { skidpad_msgs__msg__ControlCommand__Sequence__fini(seq as *mut _) }
  }
  fn sequence_copy(in_seq: &rosidl_runtime_rs::Sequence<Self>, out_seq: &mut rosidl_runtime_rs::Sequence<Self>) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { skidpad_msgs__msg__ControlCommand__Sequence__copy(in_seq, out_seq as *mut _) }
  }
}

impl rosidl_runtime_rs::Message for ControlCommand {
  type RmwMsg = Self;
  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> { msg_cow }
  fn from_rmw_message(msg: Self::RmwMsg) -> Self { msg }
}

impl rosidl_runtime_rs::RmwMessage for ControlCommand where Self: Sized {
  const TYPE_NAME: &'static str = "skidpad_msgs/msg/ControlCommand";
  fn get_type_support() -> *const std::ffi::c_void {
    // SAFETY: No preconditions for this function.
    unsafe { rosidl_typesupport_c__get_message_type_support_handle__skidpad_msgs__msg__ControlCommand() }
  }
}


#[link(name = "skidpad_msgs__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_message_type_support_handle__skidpad_msgs__msg__Trajectory() -> *const std::ffi::c_void;
}

#[link(name = "skidpad_msgs__rosidl_generator_c")]
extern "C" {
    fn skidpad_msgs__msg__Trajectory__init(msg: *mut Trajectory) -> bool;
    fn skidpad_msgs__msg__Trajectory__Sequence__init(seq: *mut rosidl_runtime_rs::Sequence<Trajectory>, size: usize) -> bool;
    fn skidpad_msgs__msg__Trajectory__Sequence__fini(seq: *mut rosidl_runtime_rs::Sequence<Trajectory>);
    fn skidpad_msgs__msg__Trajectory__Sequence__copy(in_seq: &rosidl_runtime_rs::Sequence<Trajectory>, out_seq: *mut rosidl_runtime_rs::Sequence<Trajectory>) -> bool;
}

// Corresponds to skidpad_msgs__msg__Trajectory
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]

/// 轨迹消息，由一系列轨迹点组成

#[repr(C)]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct Trajectory {

    // This member is not documented.
    #[allow(missing_docs)]
    pub header: std_msgs::msg::rmw::Header,

    /// 轨迹点数组
    pub trajectory: rosidl_runtime_rs::Sequence<super::super::msg::rmw::TrajectoryPoint>,

    /// 当前车辆位置到全局轨迹终点的弧长距离（米）
    pub remaining_distance: std_msgs::msg::rmw::Float64,

}



impl Default for Trajectory {
  fn default() -> Self {
    unsafe {
      let mut msg = std::mem::zeroed();
      if !skidpad_msgs__msg__Trajectory__init(&mut msg as *mut _) {
        panic!("Call to skidpad_msgs__msg__Trajectory__init() failed");
      }
      msg
    }
  }
}

impl rosidl_runtime_rs::SequenceAlloc for Trajectory {
  fn sequence_init(seq: &mut rosidl_runtime_rs::Sequence<Self>, size: usize) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { skidpad_msgs__msg__Trajectory__Sequence__init(seq as *mut _, size) }
  }
  fn sequence_fini(seq: &mut rosidl_runtime_rs::Sequence<Self>) {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { skidpad_msgs__msg__Trajectory__Sequence__fini(seq as *mut _) }
  }
  fn sequence_copy(in_seq: &rosidl_runtime_rs::Sequence<Self>, out_seq: &mut rosidl_runtime_rs::Sequence<Self>) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { skidpad_msgs__msg__Trajectory__Sequence__copy(in_seq, out_seq as *mut _) }
  }
}

impl rosidl_runtime_rs::Message for Trajectory {
  type RmwMsg = Self;
  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> { msg_cow }
  fn from_rmw_message(msg: Self::RmwMsg) -> Self { msg }
}

impl rosidl_runtime_rs::RmwMessage for Trajectory where Self: Sized {
  const TYPE_NAME: &'static str = "skidpad_msgs/msg/Trajectory";
  fn get_type_support() -> *const std::ffi::c_void {
    // SAFETY: No preconditions for this function.
    unsafe { rosidl_typesupport_c__get_message_type_support_handle__skidpad_msgs__msg__Trajectory() }
  }
}


#[link(name = "skidpad_msgs__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_message_type_support_handle__skidpad_msgs__msg__TrajectoryPoint() -> *const std::ffi::c_void;
}

#[link(name = "skidpad_msgs__rosidl_generator_c")]
extern "C" {
    fn skidpad_msgs__msg__TrajectoryPoint__init(msg: *mut TrajectoryPoint) -> bool;
    fn skidpad_msgs__msg__TrajectoryPoint__Sequence__init(seq: *mut rosidl_runtime_rs::Sequence<TrajectoryPoint>, size: usize) -> bool;
    fn skidpad_msgs__msg__TrajectoryPoint__Sequence__fini(seq: *mut rosidl_runtime_rs::Sequence<TrajectoryPoint>);
    fn skidpad_msgs__msg__TrajectoryPoint__Sequence__copy(in_seq: &rosidl_runtime_rs::Sequence<TrajectoryPoint>, out_seq: *mut rosidl_runtime_rs::Sequence<TrajectoryPoint>) -> bool;
}

// Corresponds to skidpad_msgs__msg__TrajectoryPoint
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]

/// 单个轨迹点信息（车体坐标系）

#[repr(C)]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct TrajectoryPoint {
    /// 坐标 (x 向前, y 向左)
    pub pts: geometry_msgs::msg::rmw::Point,

    /// 期望航向角
    pub yaw: std_msgs::msg::rmw::Float64,

    /// 路径曲率
    pub curvature: std_msgs::msg::rmw::Float64,

    /// 期望速度
    pub velocity: std_msgs::msg::rmw::Float64,

    /// 横摆角速度（预留）
    pub r: std_msgs::msg::rmw::Float64,

    /// 期望加速度（预留）
    pub acc: std_msgs::msg::rmw::Float64,

}



impl Default for TrajectoryPoint {
  fn default() -> Self {
    unsafe {
      let mut msg = std::mem::zeroed();
      if !skidpad_msgs__msg__TrajectoryPoint__init(&mut msg as *mut _) {
        panic!("Call to skidpad_msgs__msg__TrajectoryPoint__init() failed");
      }
      msg
    }
  }
}

impl rosidl_runtime_rs::SequenceAlloc for TrajectoryPoint {
  fn sequence_init(seq: &mut rosidl_runtime_rs::Sequence<Self>, size: usize) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { skidpad_msgs__msg__TrajectoryPoint__Sequence__init(seq as *mut _, size) }
  }
  fn sequence_fini(seq: &mut rosidl_runtime_rs::Sequence<Self>) {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { skidpad_msgs__msg__TrajectoryPoint__Sequence__fini(seq as *mut _) }
  }
  fn sequence_copy(in_seq: &rosidl_runtime_rs::Sequence<Self>, out_seq: &mut rosidl_runtime_rs::Sequence<Self>) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { skidpad_msgs__msg__TrajectoryPoint__Sequence__copy(in_seq, out_seq as *mut _) }
  }
}

impl rosidl_runtime_rs::Message for TrajectoryPoint {
  type RmwMsg = Self;
  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> { msg_cow }
  fn from_rmw_message(msg: Self::RmwMsg) -> Self { msg }
}

impl rosidl_runtime_rs::RmwMessage for TrajectoryPoint where Self: Sized {
  const TYPE_NAME: &'static str = "skidpad_msgs/msg/TrajectoryPoint";
  fn get_type_support() -> *const std::ffi::c_void {
    // SAFETY: No preconditions for this function.
    unsafe { rosidl_typesupport_c__get_message_type_support_handle__skidpad_msgs__msg__TrajectoryPoint() }
  }
}


