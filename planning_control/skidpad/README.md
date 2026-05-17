# Skidpad Workspace - 8字绕环规控系统

## 坐标系

| # | 名称 | 原点 | X轴 | Y轴 | 单位 | 来源 | 用于 |
|---|------|------|-----|-----|------|------|------|
| 1 | UE4 世界 | 地图原点 | 北(直道) | 东(右) | cm | FSDS 引擎 | `getRefereeState()` 锥桶绝对位置, PlayerStart |
| 2 | 车出生点 (getCarState返回) | 车出生点 | 北(直道) | 东(右) | m | FSDS `getCarState()` | bridge 读入车辆位姿 |
| 3 | **目标坐标系** | 车出生点 | 北(直道) | **西(左)** | m | **我们定义** | PCD地图, bridge发布锥桶/车位, 全局轨迹, ICP map |

**坐标系 3 是统一出口**——bridge 把所有信息都转成坐标系 3 再发布，下游模块全程使用同一个坐标系。

### 坐标转换流程

```
getRefereeState(): UE4世界坐标系1 (cm)
    → 减车出生点 → 转m → 坐标系2
    → tx=北, ty=-东(西) → 目标坐标系3 (存入PCD / 发布锥桶)

getCarState(): 车出生点坐标系2
    → px=北, py=-东(西) → 目标坐标系3 (发布车位)

genTraj(): 直道沿X, 左沿Y → 目标坐标系3 (全局轨迹)
```

## 架构

```
FSDS仿真器 ←→ [fsds_ros_bridge] ←→ ROS2 Topics
                      |
      原始雷达 → /lidar_points → [lidar_cone_detector] → /hesai/cone_positions → [ICP]
      CarState → /estimation/slam/state → [Planner] + [Controller]
      Bool → /go_cmd → [Controller]
      [Controller] → ControlCommand → /control_command → FSDS
```

## 项目结构

| 包 | 语言 | 功能 |
|----|------|------|
| `skidpad_msgs` | C++ | 自定义 ROS2 消息 |
| `skidpad_icp` | C++ | ICP 定位 |
| `skidpad_planner` | C++ | 8字路径规划 |
| `skidpad_control` | Python | 纯跟踪+纵向PID |
| `skidpad_bringup` | C++ | 启动文件 |
| `fsds_ros_bridge` | Python | FSDS ↔ ROS2 |

## 构建

```bash
source /opt/ros/humble/setup.bash
colcon build --symlink-install
source install/setup.bash
```

## 运行

本 workspace 提供 launch 文件。完整启动需先 source 其他 workspace（见根目录 README.md）。

```bash
ros2 launch skidpad_bringup skidpad_bringup.launch.py mode:=simulation
ros2 run skidpad_control skidpad_visualizer
```

## 工具

| 脚本 | 用途 |
|------|------|
| `src/skidpad_bringup/tools/generate_pcd_from_fsds.py` | 从仿真器生成 PCD 地图 |
| `src/skidpad_bringup/tools/check_spawn_distance.py` | 验证发车点到起点线距离 |

## 迁移路径

如果将工作空间移动到其他位置，修改以下硬编码路径：

| 文件 | 参数 |
|------|------|
| `src/skidpad_icp/config/skidpad_detector.yaml` | `skidpad_map` |
| `src/icp_test_tools/config/test_params.yaml` | `pcd_path` |
| `src/icp_test_tools/icp_test_tools/publisher_node.py` | `pcd_path` |
