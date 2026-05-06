# skidpad_icp - ICP 定位

通过 PCL ICP 将预加载的锥桶地图 (PCD) 与实时检测的锥桶点云对齐，发布 4x4 变换矩阵。

## 坐标系

PCD 文件和实时锥桶点云都使用**目标坐标系**：X=北(直道), Y=西(左侧), 原点=车出生点。

ICP 匹配结果为恒等变换（仿真中 PCD 和实时锥桶来源相同），发布单位矩阵。

## 数据流

```
/perception/lidar_cluster (PointCloud2) → [ICP] → /transform_matrix (Float64MultiArray)
```

## 关键文件

| 文件 | 功能 |
|------|------|
| `src/skidpad_detector.cpp` | PCD加载(loadFiles), ICP运行(runAlgorithm) |
| `src/skidpad_detector_handle.cpp` | ROS2 节点: 话题订阅/发布, 定时器 |
| `config/skidpad_detector.yaml` | ICP参数和PCD路径配置 |
| `config/skidpad.pcd` | 锥桶地图 (目标坐标系, generate_pcd_from_fsds.py 生成) |

## ICP 参数

```yaml
icp:
  max_correspondence_distance: 2.0   # 最大对应点距离 (m)
  max_iterations: 100                # 最大迭代次数
  transformation_epsilon: 1e-10      # 收敛阈值
  euclidean_fitness_epsilon: 0.001   # 均方误差阈值
```
