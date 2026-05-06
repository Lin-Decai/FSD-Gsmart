#[cfg(feature = "serde")]
use serde::{Deserialize, Serialize};



// Corresponds to skidpad_msgs__msg__CarState
/// 车辆状态主消息

#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct CarState {

    // This member is not documented.
    #[allow(missing_docs)]
    pub header: std_msgs::msg::Header,

    /// 位置和姿态角 (x, y, theta)
    pub car_state: geometry_msgs::msg::Pose2D,

    /// 一阶导数信息 (速度、加速度)
    pub car_state_dt: super::msg::CarStateDt,

}



impl Default for CarState {
  fn default() -> Self {
    <Self as rosidl_runtime_rs::Message>::from_rmw_message(super::msg::rmw::CarState::default())
  }
}

impl rosidl_runtime_rs::Message for CarState {
  type RmwMsg = super::msg::rmw::CarState;

  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> {
    match msg_cow {
      std::borrow::Cow::Owned(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        header: std_msgs::msg::Header::into_rmw_message(std::borrow::Cow::Owned(msg.header)).into_owned(),
        car_state: geometry_msgs::msg::Pose2D::into_rmw_message(std::borrow::Cow::Owned(msg.car_state)).into_owned(),
        car_state_dt: super::msg::CarStateDt::into_rmw_message(std::borrow::Cow::Owned(msg.car_state_dt)).into_owned(),
      }),
      std::borrow::Cow::Borrowed(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        header: std_msgs::msg::Header::into_rmw_message(std::borrow::Cow::Borrowed(&msg.header)).into_owned(),
        car_state: geometry_msgs::msg::Pose2D::into_rmw_message(std::borrow::Cow::Borrowed(&msg.car_state)).into_owned(),
        car_state_dt: super::msg::CarStateDt::into_rmw_message(std::borrow::Cow::Borrowed(&msg.car_state_dt)).into_owned(),
      })
    }
  }

  fn from_rmw_message(msg: Self::RmwMsg) -> Self {
    Self {
      header: std_msgs::msg::Header::from_rmw_message(msg.header),
      car_state: geometry_msgs::msg::Pose2D::from_rmw_message(msg.car_state),
      car_state_dt: super::msg::CarStateDt::from_rmw_message(msg.car_state_dt),
    }
  }
}


// Corresponds to skidpad_msgs__msg__CarStateDt
/// 车辆状态的时间导数

#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct CarStateDt {

    // This member is not documented.
    #[allow(missing_docs)]
    pub header: std_msgs::msg::Header,

    /// 速度 (x, y, theta)
    pub car_state_dt: geometry_msgs::msg::Pose2D,

    /// 加速度 (x, y, theta)
    pub car_state_a: geometry_msgs::msg::Pose2D,

}



impl Default for CarStateDt {
  fn default() -> Self {
    <Self as rosidl_runtime_rs::Message>::from_rmw_message(super::msg::rmw::CarStateDt::default())
  }
}

impl rosidl_runtime_rs::Message for CarStateDt {
  type RmwMsg = super::msg::rmw::CarStateDt;

  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> {
    match msg_cow {
      std::borrow::Cow::Owned(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        header: std_msgs::msg::Header::into_rmw_message(std::borrow::Cow::Owned(msg.header)).into_owned(),
        car_state_dt: geometry_msgs::msg::Pose2D::into_rmw_message(std::borrow::Cow::Owned(msg.car_state_dt)).into_owned(),
        car_state_a: geometry_msgs::msg::Pose2D::into_rmw_message(std::borrow::Cow::Owned(msg.car_state_a)).into_owned(),
      }),
      std::borrow::Cow::Borrowed(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        header: std_msgs::msg::Header::into_rmw_message(std::borrow::Cow::Borrowed(&msg.header)).into_owned(),
        car_state_dt: geometry_msgs::msg::Pose2D::into_rmw_message(std::borrow::Cow::Borrowed(&msg.car_state_dt)).into_owned(),
        car_state_a: geometry_msgs::msg::Pose2D::into_rmw_message(std::borrow::Cow::Borrowed(&msg.car_state_a)).into_owned(),
      })
    }
  }

  fn from_rmw_message(msg: Self::RmwMsg) -> Self {
    Self {
      header: std_msgs::msg::Header::from_rmw_message(msg.header),
      car_state_dt: geometry_msgs::msg::Pose2D::from_rmw_message(msg.car_state_dt),
      car_state_a: geometry_msgs::msg::Pose2D::from_rmw_message(msg.car_state_a),
    }
  }
}


// Corresponds to skidpad_msgs__msg__ControlCommand
/// 控制命令

#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct ControlCommand {

    // This member is not documented.
    #[allow(missing_docs)]
    pub header: std_msgs::msg::Header,

    /// 油门值，范围[-1, 1]，正为加速，负为制动
    pub throttle: std_msgs::msg::Float32,

    /// 转向角，范围[-1, 1]，左负右正
    pub steering_angle: std_msgs::msg::Float32,

}



impl Default for ControlCommand {
  fn default() -> Self {
    <Self as rosidl_runtime_rs::Message>::from_rmw_message(super::msg::rmw::ControlCommand::default())
  }
}

impl rosidl_runtime_rs::Message for ControlCommand {
  type RmwMsg = super::msg::rmw::ControlCommand;

  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> {
    match msg_cow {
      std::borrow::Cow::Owned(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        header: std_msgs::msg::Header::into_rmw_message(std::borrow::Cow::Owned(msg.header)).into_owned(),
        throttle: std_msgs::msg::Float32::into_rmw_message(std::borrow::Cow::Owned(msg.throttle)).into_owned(),
        steering_angle: std_msgs::msg::Float32::into_rmw_message(std::borrow::Cow::Owned(msg.steering_angle)).into_owned(),
      }),
      std::borrow::Cow::Borrowed(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        header: std_msgs::msg::Header::into_rmw_message(std::borrow::Cow::Borrowed(&msg.header)).into_owned(),
        throttle: std_msgs::msg::Float32::into_rmw_message(std::borrow::Cow::Borrowed(&msg.throttle)).into_owned(),
        steering_angle: std_msgs::msg::Float32::into_rmw_message(std::borrow::Cow::Borrowed(&msg.steering_angle)).into_owned(),
      })
    }
  }

  fn from_rmw_message(msg: Self::RmwMsg) -> Self {
    Self {
      header: std_msgs::msg::Header::from_rmw_message(msg.header),
      throttle: std_msgs::msg::Float32::from_rmw_message(msg.throttle),
      steering_angle: std_msgs::msg::Float32::from_rmw_message(msg.steering_angle),
    }
  }
}


// Corresponds to skidpad_msgs__msg__Trajectory
/// 轨迹消息，由一系列轨迹点组成

#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct Trajectory {

    // This member is not documented.
    #[allow(missing_docs)]
    pub header: std_msgs::msg::Header,

    /// 轨迹点数组
    pub trajectory: Vec<super::msg::TrajectoryPoint>,

    /// 当前车辆位置到全局轨迹终点的弧长距离（米）
    pub remaining_distance: std_msgs::msg::Float64,

}



impl Default for Trajectory {
  fn default() -> Self {
    <Self as rosidl_runtime_rs::Message>::from_rmw_message(super::msg::rmw::Trajectory::default())
  }
}

impl rosidl_runtime_rs::Message for Trajectory {
  type RmwMsg = super::msg::rmw::Trajectory;

  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> {
    match msg_cow {
      std::borrow::Cow::Owned(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        header: std_msgs::msg::Header::into_rmw_message(std::borrow::Cow::Owned(msg.header)).into_owned(),
        trajectory: msg.trajectory
          .into_iter()
          .map(|elem| super::msg::TrajectoryPoint::into_rmw_message(std::borrow::Cow::Owned(elem)).into_owned())
          .collect(),
        remaining_distance: std_msgs::msg::Float64::into_rmw_message(std::borrow::Cow::Owned(msg.remaining_distance)).into_owned(),
      }),
      std::borrow::Cow::Borrowed(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        header: std_msgs::msg::Header::into_rmw_message(std::borrow::Cow::Borrowed(&msg.header)).into_owned(),
        trajectory: msg.trajectory
          .iter()
          .map(|elem| super::msg::TrajectoryPoint::into_rmw_message(std::borrow::Cow::Borrowed(elem)).into_owned())
          .collect(),
        remaining_distance: std_msgs::msg::Float64::into_rmw_message(std::borrow::Cow::Borrowed(&msg.remaining_distance)).into_owned(),
      })
    }
  }

  fn from_rmw_message(msg: Self::RmwMsg) -> Self {
    Self {
      header: std_msgs::msg::Header::from_rmw_message(msg.header),
      trajectory: msg.trajectory
          .into_iter()
          .map(super::msg::TrajectoryPoint::from_rmw_message)
          .collect(),
      remaining_distance: std_msgs::msg::Float64::from_rmw_message(msg.remaining_distance),
    }
  }
}


// Corresponds to skidpad_msgs__msg__TrajectoryPoint
/// 单个轨迹点信息（车体坐标系）

#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct TrajectoryPoint {
    /// 坐标 (x 向前, y 向左)
    pub pts: geometry_msgs::msg::Point,

    /// 期望航向角
    pub yaw: std_msgs::msg::Float64,

    /// 路径曲率
    pub curvature: std_msgs::msg::Float64,

    /// 期望速度
    pub velocity: std_msgs::msg::Float64,

    /// 横摆角速度（预留）
    pub r: std_msgs::msg::Float64,

    /// 期望加速度（预留）
    pub acc: std_msgs::msg::Float64,

}



impl Default for TrajectoryPoint {
  fn default() -> Self {
    <Self as rosidl_runtime_rs::Message>::from_rmw_message(super::msg::rmw::TrajectoryPoint::default())
  }
}

impl rosidl_runtime_rs::Message for TrajectoryPoint {
  type RmwMsg = super::msg::rmw::TrajectoryPoint;

  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> {
    match msg_cow {
      std::borrow::Cow::Owned(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        pts: geometry_msgs::msg::Point::into_rmw_message(std::borrow::Cow::Owned(msg.pts)).into_owned(),
        yaw: std_msgs::msg::Float64::into_rmw_message(std::borrow::Cow::Owned(msg.yaw)).into_owned(),
        curvature: std_msgs::msg::Float64::into_rmw_message(std::borrow::Cow::Owned(msg.curvature)).into_owned(),
        velocity: std_msgs::msg::Float64::into_rmw_message(std::borrow::Cow::Owned(msg.velocity)).into_owned(),
        r: std_msgs::msg::Float64::into_rmw_message(std::borrow::Cow::Owned(msg.r)).into_owned(),
        acc: std_msgs::msg::Float64::into_rmw_message(std::borrow::Cow::Owned(msg.acc)).into_owned(),
      }),
      std::borrow::Cow::Borrowed(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        pts: geometry_msgs::msg::Point::into_rmw_message(std::borrow::Cow::Borrowed(&msg.pts)).into_owned(),
        yaw: std_msgs::msg::Float64::into_rmw_message(std::borrow::Cow::Borrowed(&msg.yaw)).into_owned(),
        curvature: std_msgs::msg::Float64::into_rmw_message(std::borrow::Cow::Borrowed(&msg.curvature)).into_owned(),
        velocity: std_msgs::msg::Float64::into_rmw_message(std::borrow::Cow::Borrowed(&msg.velocity)).into_owned(),
        r: std_msgs::msg::Float64::into_rmw_message(std::borrow::Cow::Borrowed(&msg.r)).into_owned(),
        acc: std_msgs::msg::Float64::into_rmw_message(std::borrow::Cow::Borrowed(&msg.acc)).into_owned(),
      })
    }
  }

  fn from_rmw_message(msg: Self::RmwMsg) -> Self {
    Self {
      pts: geometry_msgs::msg::Point::from_rmw_message(msg.pts),
      yaw: std_msgs::msg::Float64::from_rmw_message(msg.yaw),
      curvature: std_msgs::msg::Float64::from_rmw_message(msg.curvature),
      velocity: std_msgs::msg::Float64::from_rmw_message(msg.velocity),
      r: std_msgs::msg::Float64::from_rmw_message(msg.r),
      acc: std_msgs::msg::Float64::from_rmw_message(msg.acc),
    }
  }
}


