# 吉智无人车队 · 无人系统工作空间

2026 年吉林大学吉智无人方程式车队（GSmart）无人系统工作空间，面向 FSAC 比赛。

## 模块

| 模块 | 路径 | 说明 |
|------|------|------|
| 感知 | `sensor_perception/` | 激光雷达锥桶检测，后续将加入摄像头感知 |
| 定位建图 | `slam_localization/` | IMU + GNSS 位姿估计，SLAM 建图 |
| 规控 | `planning_control/` | 8 字绕环路径规划、纯跟踪横向控制 + PID 纵向控制 |

## 环境

- Ubuntu 22.04
- ROS 2 Humble
- FSDS 仿真器 (Formula Student Driverless Simulator)

## 构建

```bash
cd planning_control/skidpad_ws
source /opt/ros/humble/setup.bash
colcon build --symlink-install
source install/setup.bash
```

## 运行

```bash
# 仿真模式
ros2 launch skidpad_bringup skidpad_bringup.launch.py mode:=simulation

# 可视化
ros2 run skidpad_control skidpad_visualizer
```
