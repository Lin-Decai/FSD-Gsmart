# CHANGELOG

## 2026-05-17

### 架构重构：FSDS 桥接 + 雷达聚类 + ICP 全链路打通

**目标**：用雷达聚类模块替代原来从仿真器直接获取锥桶坐标的方式，构建完整的仿真→感知→规控链路。

---

### 一、新建 `fsds_ros_bridge` 替代旧 `fsds_bridge`

| 文件 | 操作 | 说明 |
|------|------|------|
| `fsds_ros_bridge/` (7个文件) | **新建** | 通用 FSDS→ROS 桥接包，发布原始雷达点云 `/lidar_points`、车辆状态 `/estimation/slam/state`、GO 信号 `/go_cmd` |
| `planning_control/skidpad/src/fsds_bridge/` | **删除** | 旧桥接包，只发锥桶坐标，不经过雷达模块 |

关键实现：
- `getLidarData()` 获取原始雷达点云，世界坐标系→传感器坐标系 (四元数逆旋转 + Y轴翻转)
- 发布 PointCloud2 含 x/y/z/intensity 四字段，兼容 PCL `PointXYZI`
- QoS 使用 `SensorDataQoS` 匹配雷达聚类订阅
- ICP 成功后自动停止 lidar 定时器，释放 CPU
- 状态发布频率 100→30Hz，雷达发布频率 10→5Hz

### 二、ICP 模块适配雷达聚类

| 文件 | 修改 |
|------|------|
| `skidpad_icp/include/skidpad_detector_handle.hpp` | 订阅类型 `PointCloud2→PoseArray`，话题 `/perception/lidar_cluster→/hesai/cone_positions` |
| `skidpad_icp/src/skidpad_detector_handle.cpp` | PoseArray→PointCloud2 转换回调，`RCLCPP_INFO_ONCE` 打印接收确认 |
| `skidpad_icp/src/skidpad_detector.cpp` | `loadFiles()` 和 `runAlgorithm()` 各加 `lidar2imu` X 偏移；`RCLCPP_WARN→WARN_ONCE` 防刷屏 |
| `skidpad_icp/config/skidpad_detector.yaml` | 话题名改为 `/hesai/cone_positions`，`lidar2imu: 1.2225` |

### 三、轨迹生成修正

| 文件 | 修改 |
|------|------|
| `skidpad_planner/src/Track/skidpad_track.cpp` | 切点/圆心 X 坐标从 `car_length+forward_distance(16.88)` 改为 `forward_distance(15.00)`，符合赛规 |

### 四、可视化适配

| 文件 | 修改 |
|------|------|
| `skidpad_control/skidpad_control/visualize.py` | 锥桶订阅从 `/perception/lidar_cluster`(PointCloud2) 改为 `/hesai/cone_positions`(PoseArray) |

### 五、雷达聚类参数 YAML 化

| 文件 | 操作 | 说明 |
|------|------|------|
| `lidar/lidar_cone_detector/src/lidar_cone_detector_3.19.cpp` | 修改 | `min_size`/`max_size`/`tolerance` 改为 ROS 参数 |
| `lidar/lidar_cone_detector/config/lidar_cone_detector.yaml` | **新建** | 仿真: `min_size=5`，实车改 `10` |
| `lidar/lidar_cone_detector/CMakeLists.txt` | 修改 | install config 目录 |
| `skidpad_bringup/launch/skidpad_bringup.launch.py` | 修改 | 传入 YAML 给雷达聚类；新增 `lidar_cone_detector` 节点 |
| `lidar/lidar_cone_detector/tools/record_cones.py` | **新建** | 锥桶录制工具，用于聚类精度验证 |
| `lidar/lidar_cone_detector/README.md` | **新建** | 雷达聚类模块说明 |
| `planning_control/skidpad/src/skidpad_bringup/tools/check_spawn_distance.py` | **新建** | 发车点到起点线距离验证工具 |

### 六、FSDS 仿真卡顿排查与解决

**根因**：UE4 中 AirSim 雷达用 `LineTraceSingleByChannel` 单线程同步光线追踪。124 线 × 153600 点 × 10Hz = 153 万光线/秒，远超官方建议上限 (~10 万/秒)，阻塞游戏主线程。

**解法**：`PointsPerScan: 60000`，`RotationsPerSecond: 1`。总光线 6 万/秒，卡在官方建议内。1Hz 扫描供 ICP 算一帧变换，之后 bridge 自动停 lidar 管线。

### 七、目录重构

| 旧路径 | 新路径 |
|--------|--------|
| `planning_control/skidpad_ws/` | `planning_control/skidpad/` |
| `lidar_cone_detector/` | `lidar/lidar_cone_detector/` |
| `li_cam_percep/` | `camera/` |

### 当前完整编译步骤

```bash
# 1. 规控
cd ~/GSMART/CODE/FSD-Gsmart/planning_control/skidpad && source /opt/ros/humble/setup.bash && colcon build --symlink-install

# 2. FSDS 桥接
cd ~/GSMART/CODE/FSD-Gsmart/fsds_ros_bridge && source /opt/ros/humble/setup.bash && source ~/GSMART/CODE/FSD-Gsmart/planning_control/skidpad/install/setup.bash && colcon build --symlink-install

# 3. 雷达聚类
cd ~/GSMART/CODE/FSD-Gsmart/lidar/lidar_cone_detector && source /opt/ros/humble/setup.bash && colcon build --symlink-install
```

### 当前启动步骤（1个终端）

```bash
source /opt/ros/humble/setup.bash && \
  source ~/GSMART/CODE/FSD-Gsmart/lidar/lidar_cone_detector/install/setup.bash && \
  source ~/GSMART/CODE/FSD-Gsmart/fsds_ros_bridge/install/setup.bash && \
  source ~/GSMART/CODE/FSD-Gsmart/planning_control/skidpad/install/setup.bash && \
  ros2 launch skidpad_bringup skidpad_bringup.launch.py mode:=simulation
```

### 当前 settings.json 关键值

```json
"Lidar1": { "NumberOfLasers": 124, "PointsPerScan": 60000, "RotationsPerSecond": 1, ... }
"Vehicles": { "FSCar": { "X": 0.0, "Y": 0.0, ... } }
```

### 当前关键参数

| 参数 | 值 | 位置 |
|------|-----|------|
| lidar2imu | 1.2225 | `skidpad_detector.yaml` |
| forward_distance | 15.0 | `skidpad_planner.yaml` |
| 聚类 min_size | 5 | `lidar_cone_detector.yaml` |
| PCD X 偏移 | -1.8 | `skidpad.pcd` (手动修改) |

---

### ⚠️ 重要说明：仿真与实车的参数隔离

**仿真器限制**：FSDS 的 UE4 雷达光线追踪跑在游戏主线程上，124 线高分辨率会导致仿真卡顿。我们被迫降低了 `PointsPerScan`、`RotationsPerSecond` 等参数来适应仿真器性能（官方建议上限 ~100,000 点/秒）。这些修改**仅服务于仿真环境**。

**参数隔离策略**：
- `settings.json`：仿真器专属配置，随便改，不影响实车
- **雷达聚类 C++ 源码** (`lidar_cone_detector_3.19.cpp`)：实车已验证的参数**一个都没动**——硬编码的原始值 (`minClusterSize=10`、`maxClusterSize=2000`、`clusterTolerance=0.5`) 作为 ROS 参数的**默认值**保留，实车启动时用默认值
- **YAML 文件** (`lidar_cone_detector/config/lidar_cone_detector.yaml`)：通过 ROS 参数机制**覆盖**默认值，仅仿真 launch 加载。实车启动时不传 YAML 或传另一个实车 YAML 即可恢复原始参数
