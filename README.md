# 吉智无人车队 · 无人系统工作空间

2026 年吉林大学吉智无人方程式车队（GSmart）无人系统工作空间，面向 FSAC 比赛。

## 模块

| 模块 | 路径 | 说明 |
|------|------|------|
| 规控 | `planning_control/skidpad/` | 8 字绕环 ICP 定位 + 路径规划 + 纯跟踪控制 |
| 雷达感知 | `lidar/lidar_cone_detector/` | 激光雷达锥桶检测 (ROI→RANSAC→聚类→形态学) |
| 相机感知 | `camera/` | YOLO 目标检测 + 激光雷达-相机融合 |
| 仿真桥接 | `fsds_ros_bridge/` | FSDS 仿真器 ↔ ROS 2 (原始雷达 + 车辆状态 + 控制) |
| 定位建图 | `slam_localization/` | IMU + GNSS 位姿估计，SLAM 建图 |

## 环境

- Ubuntu 22.04
- ROS 2 Humble
- FSDS 仿真器 (Formula Student Driverless Simulator)

## 构建

按依赖顺序编译 **3 个工作空间**：

```bash
# 1. 规控工作空间（先编译，fsds_ros_bridge 依赖 skidpad_msgs）
cd ~/GSMART/CODE/FSD-Gsmart/planning_control/skidpad
source /opt/ros/humble/setup.bash
colcon build --symlink-install

# 2. FSDS 桥接（依赖 skidpad_msgs）
cd ~/GSMART/CODE/FSD-Gsmart/fsds_ros_bridge
source /opt/ros/humble/setup.bash
source ~/GSMART/CODE/FSD-Gsmart/planning_control/skidpad/install/setup.bash
colcon build --symlink-install

# 3. 雷达锥桶检测
cd ~/GSMART/CODE/FSD-Gsmart/lidar/lidar_cone_detector
source /opt/ros/humble/setup.bash
colcon build --symlink-install
```

## 运行

FSDS 仿真器先启动，然后一个终端跑全部：

```bash
source /opt/ros/humble/setup.bash && \
  source ~/GSMART/CODE/FSD-Gsmart/lidar/lidar_cone_detector/install/setup.bash && \
  source ~/GSMART/CODE/FSD-Gsmart/fsds_ros_bridge/install/setup.bash && \
  source ~/GSMART/CODE/FSD-Gsmart/planning_control/skidpad/install/setup.bash && \
  ros2 launch skidpad_bringup skidpad_bringup.launch.py mode:=simulation
```

可视化（另开终端）：

```bash
source /opt/ros/humble/setup.bash && \
  source ~/GSMART/CODE/FSD-Gsmart/planning_control/skidpad/install/setup.bash && \
  ros2 run skidpad_control skidpad_visualizer
```

## 文档

| 文件 | 内容 |
|------|------|
| `CHANGELOG.md` | 修改日志 |
| `TODO.md` | 待解决问题 |
| `CLAUDE.md` | 项目技术说明 |
| `planning_control/skidpad/CLAUDE.md` | 规控算法详解 |
