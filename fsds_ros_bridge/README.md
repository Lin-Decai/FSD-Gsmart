# fsds_ros_bridge — FSDS 仿真器 ↔ ROS 2 桥接

将 FSDS 仿真器的原始雷达点云、车辆状态、控制指令在 ROS 2 话题上中转。

## 数据流

```
FSDS 仿真器
  │
  ├── getLidarData('Lidar1')  ──→  /lidar_points (PointCloud2, hesai_lidar 帧)
  ├── getCarState()           ──→  /estimation/slam/state (CarState, map 帧)
  ├── (启动时)                ──→  /go_cmd (Bool)
  │
  └── setCarControls()        ←──  /control_command (ControlCommand)
```

## 话题

| 方向 | 话题 | 类型 | 频率 | 说明 |
|------|------|------|------|------|
| 发布 | `/lidar_points` | PointCloud2 | 5 Hz | 原始雷达点云，传感器坐标系，含 intensity=0 |
| 发布 | `/estimation/slam/state` | CarState | 30 Hz | 车辆位姿/速度，目标坐标系 (X=北, Y=西) |
| 发布 | `/go_cmd` | Bool | 启动时 1 次 | True 信号触发控制器 |
| 订阅 | `/control_command` | ControlCommand | — | 油门 [-1,1] / 转向 [-1,1]，转发给仿真器 |

## 坐标变换

### 雷达点云：世界 → 传感器坐标系

```
getLidarData() 返回:
  point_cloud: 世界坐标系 (AirSim: X=北, Y=东, Z=上)
  pose:        雷达在世界坐标系的位姿

变换步骤:
  1. pts_rel = pts_world - lidar_pos        (平移)
  2. pts_airsim = pts_rel @ R               (旋转到传感器系, AirSim 约定)
  3. pts_ros[:,1] = -pts_airsim[:,1]        (Y轴翻: 右→左, 转为 ROS 约定)

输出: hesai_lidar 帧 (X=前, Y=左, Z=上)
```

### 车辆状态：ENU → 目标坐标系

```
getCarState() 返回: X=北, Y=东, yaw:0=北
  变换: py = -enu_y  (东→取反→西)

输出: map 帧 (X=北, Y=西)
```

## 参数

| 参数 | 默认值 | 说明 |
|------|--------|------|
| `state_rate` | 30 | 车辆状态发布频率 (Hz) |
| `lidar_rate` | 5 | 雷达点云发布频率 (Hz) |
| `lidar_name` | Lidar1 | 仿真器中雷达传感器名称 |

## ICP 完成后自动停雷达

订阅 `/transform_matrix`，一旦收到（ICP 成功），取消 lidar 定时器，释放 CPU。日志输出：

```
[fsds_bridge]: ICP transform received — lidar pipeline stopped to save CPU
```

## 依赖

- rclpy, sensor_msgs, std_msgs, geometry_msgs
- skidpad_msgs (自定义消息)
- FSDS Python 客户端 (`~/Formula-Student-Driverless-Simulator/python`)
