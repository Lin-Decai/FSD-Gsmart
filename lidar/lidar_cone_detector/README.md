# lidar_cone_detector — 激光雷达锥桶检测

订阅原始雷达点云，经过 ROI 滤波 → RANSAC 地面分割 → 欧式聚类 → 形态学过滤，发布检测到的锥桶质心坐标。

## 文件说明

### `src/lidar_cone_detector_3.19.cpp`

ROS 2 C++ 节点 `hesai_processor`，处理管线：

| 步骤 | 方法 | 说明 |
|------|------|------|
| 1 | `pointCloudCallback` | 接收 `/lidar_points` (PointCloud2)，转 PCL，剔除无效点 |
| 2 | `extractROI` | 直通滤波: X[0.8,50] Y[-3,3] Z[-1.1,0.8] |
| 3 | `groundFilter` | RANSAC 平面拟合，去除地面点 (迭代 90 次, 距离阈值 0.035m) |
| 4 | `clusterAndDetectCones` | kd-tree + 欧式聚类 (容差 0.5m, 10~2000 点/簇) + 形态学过滤 (高度 0.1~0.75m, 宽/深 ≤0.5m, 密度 ≥0.1) |
| 5 | `publishConePositions` | 发布质心到 `/hesai/cone_positions` (PoseArray)，附带耗时日志 |

**订阅**: `/lidar_points` (sensor_msgs/PointCloud2, SensorDataQoS)

**发布**:

| 话题 | 类型 | 用途 |
|------|------|------|
| `/hesai/roi_cloud` | PointCloud2 | ROI 后点云 (调试) |
| `/hesai/ground_filtered` | PointCloud2 | 去地面后点云 (调试) |
| `/hesai/clustered` | PointCloud2 | 聚类后点云 (调试) |
| `/hesai/cone_positions` | PoseArray | **锥桶质心坐标** (position=x/y/z, orientation.w=点数) |

### `tools/record_cones.py`

测试工具，订阅 `/hesai/cone_positions`，收到第一帧有效数据后写入 `tools/recorded_cones.txt`。

```
用法:
  source install/setup.bash
  python3 tools/record_cones.py

输出格式: x y z point_count
```

## 性能

点数与 `settings.json` 的 `PointsPerScan` 成正比。仿真建议 `PointsPerScan: 15360`（~1 秒/帧），真实雷达按实际参数。

## 依赖

- ROS 2 Humble
- PCL 1.10+
- sensor_msgs, geometry_msgs
