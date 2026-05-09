# 吉智无人车队 · 无人系统工作空间

2026 年吉林大学吉智无人方程式车队（GSmart）无人系统工作空间，面向 FSAC 比赛。

## 模块

| 模块 | 路径 | 说明 |
|------|------|------|
| 规控 | `planning_control/` | 8 字绕环路径规划、纯跟踪横向控制 + PID 纵向控制 |
| 雷达感知 | `lidar/` | 激光雷达锥桶检测 |
| 相机感知 | `camera/` | 摄像头感知 |
| 仿真桥接 | `fsds_ros_bridge/` | FSDS 仿真器 ↔ ROS 2 |
| 定位建图 | `slam_localization/` | IMU + GNSS 位姿估计，SLAM 建图 |

## 环境

- Ubuntu 22.04
- ROS 2 Humble
- FSDS 仿真器 (Formula Student Driverless Simulator)

## 构建

```bash
# 1. FSDS ROS Bridge
cd fsds_ros_bridge
source /opt/ros/humble/setup.bash
source planning_control/skidpad/install/setup.bash  # for skidpad_msgs
colcon build --symlink-install

# 2. 雷达锥桶检测
cd lidar/lidar_cone_detector
source /opt/ros/humble/setup.bash
colcon build --symlink-install

# 3. 规控工作空间
cd planning_control/skidpad
source /opt/ros/humble/setup.bash
colcon build --symlink-install
```

## 运行

```bash
source /opt/ros/humble/setup.bash
source lidar/lidar_cone_detector/install/setup.bash
source fsds_ros_bridge/install/setup.bash
source planning_control/skidpad/install/setup.bash

# 仿真模式
ros2 launch skidpad_bringup skidpad_bringup.launch.py mode:=simulation

# 可视化
ros2 run skidpad_control skidpad_visualizer
```
