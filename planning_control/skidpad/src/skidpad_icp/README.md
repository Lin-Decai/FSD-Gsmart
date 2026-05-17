# skidpad_icp — ICP 定位

接收雷达聚类发布的锥桶质心坐标，通过 PCL ICP 与预加载的 PCD 参考地图对齐，计算 4x4 变换矩阵。

## 数据流

```
/hesai/cone_positions (PoseArray, 传感器坐标系)
    → PoseArray→PointCloud2 转换
    → 最近邻匹配 (vs PCD 地图)
    → PCL ICP 配准
    → /transform_matrix (Float64MultiArray, 4×4 行主序)
```

## 坐标系

| 数据 | 坐标系 | 原点 |
|------|--------|------|
| PCD 参考地图 | 目标坐标系 (X=北, Y=西) | 发车点 (手动偏移 -1.8m) |
| 锥桶输入 `/hesai/cone_positions` | 传感器坐标系 (hesai_lidar) | 雷达安装位置 |

ICP 算法自动计算两坐标系间的刚体变换 (旋转+平移)，`lidar2imu` 参数在 PCD 加载和锥桶处理中各加一次，互相抵消不影响匹配。

## 关键文件

| 文件 | 功能 |
|------|------|
| `src/skidpad_detector.cpp` | PCD 加载 (`loadFiles`)、ICP 匹配 (`runAlgorithm`) |
| `src/skidpad_detector_handle.cpp` | ROS 节点：订阅 `/hesai/cone_positions` (PoseArray)，发布 `/transform_matrix` |
| `config/skidpad_detector.yaml` | 参数配置 (lidar2imu, ICP 收敛参数, 话题名) |
| `config/skidpad.pcd` | 锥桶参考地图 (ASCII PCD，目标坐标系) |

## 参数

```yaml
cluster_filtered_topic_name: /hesai/cone_positions  # 输入话题
lidar2imu: 1.2225         # 雷达前向偏移 (m)
threshold: 2.0             # 最近邻匹配距离阈值 (m)
icp:
  max_correspondence_distance: 2.0
  max_iterations: 100
```
