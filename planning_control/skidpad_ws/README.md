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
FSDS仿真器 ←→ [fsds_bridge] ←→ ROS2 Topics
                      │
      CarState → /estimation/slam/state → [Planner] + [Controller]
      PointCloud2 → /perception/lidar_cluster → [ICP]
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
| `fsds_bridge` | Python | FSDS ↔ ROS2 |

## 构建 & 运行

```bash
source /opt/ros/humble/setup.bash
colcon build --symlink-install
source install/setup.bash

# 生成PCD (首次或地图变更后需要)
python3 src/skidpad_bringup/tools/generate_pcd_from_fsds.py

# 仿真
ros2 launch skidpad_bringup skidpad_bringup.launch.py mode:=simulation

# 可视化
ros2 run skidpad_control skidpad_visualizer
```

## 迁移路径

如果将工作空间移动到其他位置，需要修改以下文件中的硬编码路径：

| 文件 | 需要修改的内容 |
|------|---------------|
| `src/skidpad_icp/config/skidpad_detector.yaml` | `skidpad_map` 的 PCD 文件路径 |
| `src/skidpad_bringup/tools/generate_pcd_from_fsds.py` | PCD 输出目录 |
| `src/icp_test_tools/config/test_params.yaml` | PCD 路径 |
| `src/icp_test_tools/icp_test_tools/publisher_node.py` | PCD 路径默认值 |

可将旧路径全部替换为新路径：
```bash
OLD=~/GSMART/CODE/skidpad_ws
NEW=~/GSMART/CODE/FSD-Gsmart/planning_control/skidpad_ws
grep -rl "$OLD" src/ | xargs sed -i "s|$OLD|$NEW|g"
```
