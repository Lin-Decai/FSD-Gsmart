# fsds_bridge - FSDS 仿真桥接

FSDS 仿真器 ↔ ROS2 的数据桥接节点。

## 坐标系转换

FSDS 内部使用 UE4 坐标系 (cm, X=北, Y=东)。桥接节点负责转换到**目标坐标系** (m, X=北, Y=西)。

```
getRefereeState() → UE4(cm) → 减车出生点 → 目标坐标系 → /perception/lidar_cluster
getCarState()     → UE4(m)  → Y取反        → 目标坐标系 → /estimation/slam/state
```

## 发布的话题

| 话题 | 类型 | 内容 |
|------|------|------|
| `/estimation/slam/state` | CarState | 车辆位姿/速度/加速度 |
| `/perception/lidar_cluster` | PointCloud2 | 80个锥桶坐标 |
| `/go_cmd` | Bool | GO信号 (仿真启动后发送) |

## 订阅的话题

| 话题 | 类型 | 内容 |
|------|------|------|
| `/control_command` | ControlCommand | 油门/转向指令 → FSDS |

## 关键参数

```python
self.declare_parameter('cone_rate', 10)     # 锥桶发布频率 (Hz)
self.declare_parameter('state_rate', 100)   # 状态发布频率 (Hz)
self.declare_parameter('add_cone_noise', False)  # 是否加噪声模拟真实感知
```
