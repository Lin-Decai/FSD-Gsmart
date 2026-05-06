# ICP 测试工具 (`icp_test_tools`) 使用手册

## 概述
`icp_test_tools` 用于验证 `skidpad_icp` 节点的 ICP 配准功能是否正确。它包含两个核心节点：

- **test_lidar_publisher**：模拟发布聚类后的锥桶中心点云（`/perception/lidar_cluster`），可以设置为发送与地图完全一致的点云（`identity` 模式）或施加已知变换后的点云（`shifted` 模式）。
- **transform_printer**：订阅 `/transform_matrix` 话题，将接收到的 4×4 变换矩阵按行打印在终端，便于观察。

配合原始的 `skidpad_icp` 节点，你可以通过对比已知变换与 ICP 输出的矩阵来判断代码是否正确。


---

## 依赖
- ROS2 Humble（或兼容版本）
- `skidpad_icp` 及 `skidpad_msgs`（自定义消息）
- `sensor_msgs`, `std_msgs`, `sensor_msgs_py`, `numpy`
- 系统已安装 `python3-numpy`

---

## 使用步骤

### 1. 编译工作空间
```bash
cd ~/GSMART/CODE/FSD-Gsmart/planning_control/skidpad_ws
colcon build --packages-select skidpad_msgs skidpad_icp icp_test_tools
source install/setup.bash
```

### 2. 配置 `test_params.yaml`
编辑 `icp_test_tools/config/test_params.yaml`，主要参数：

- `topic`：发布的点云话题，应与 `skidpad_detector.yaml` 中的 `cluster_filtered_topic_name` 一致（默认 `/perception/lidar_cluster`）。
- `publish_rate`：发布频率（Hz）。
- `mode`：测试模式，可选 `"identity"`（发送原始地图点云）或 `"shifted"`（发送经过旋转+平移的点云）。
- `transform`：仅在 `mode: "shifted"` 时有效，包含旋转（`roll`, `pitch`, `yaw`，弧度）和平移（`translation_x`, `translation_y`, `translation_z`，米）。通常只需设置 `yaw` 和 `translation_x/y`。

#### 示例：测试恒等变换
```yaml
test_lidar_publisher:
  ros__parameters:
    topic: /perception/lidar_cluster
    publish_rate: 10.0
    mode: "identity"
    # transform 参数可保持默认
```

#### 示例：测试小幅度变换（1°，0.1m 平移）
```yaml
test_lidar_publisher:
  ros__parameters:
    mode: "shifted"
    transform:
      yaw: 0.0175      # 约 1°
      translation_x: 0.1
      translation_y: -0.05
```

### 3. 确保 `skidpad_icp` 的地图文件存在
- PCD 地图文件路径应正确配置在 `skidpad_icp/config/skidpad_detector.yaml` 中。
- `path_x.txt` 和 `path_y.txt` 可以留空或不使用（ICP 仅依赖 PCD）。
- ICP 匹配阈值建议在测试时适当放宽，例如：
  ```yaml
  length:
    threshold: 20.0
  icp:
    max_correspondence_distance: 20.0
  ```

### 4. 启动测试
```bash
ros2 launch icp_test_tools icp_test.launch.py
```

终端会同时启动三个节点，观察日志输出。

---

## 终端日志说明

### publisher_node
- `Publisher ready, mode=xxx, xx points`  
  发布器启动成功，显示当前模式（identity/shifted）和点云点数（应与 PCD 文件点数一致）。
- `Publishing xx points, first 3: (x1,y1), (x2,y2), (x3,y3)`  
  每次发布时打印前三个点的坐标（单位：米），可用来确认点云是否被正确变换。  
  *identity 模式下，前三点坐标应与地图原始坐标一致；shifted 模式下，坐标应等于原始坐标经过参数指定的旋转和偏移后的值。*

### skidpad_detector_node
- `Constructing Handle` → `loading parameters` → `Loaded PCD file with xx points`  
  正常初始化日志。
- `ICP succeeded. Score: x.xxxx`  
  ICP 计算完成，Score 表示均方根误差（越小越好）。  
  **0.000000 代表完美匹配**（identity 模式或变换被完全恢复时出现）。  
  Score < 0.01 通常认为匹配极佳。
- `ICP params: max_corr_dist=xx, max_iter=xx, src=xx, tgt=xx`  
  调试日志（我们添加的），显示本次 ICP 的实际参数和匹配点对数量。`src` 与 `tgt` 应等于锥桶总数（68），否则说明有锥桶未匹配。
- 若出现 `Not enough correspondences` 或异常退出，说明匹配点对不足，请增大 `threshold` 参数。
- 若出现 `ICP did not converge`，说明算法未收敛，可调整 `icp` 参数或检查变换是否过大。

### printer_node
- `Received transformation matrix:`  
  接收到的 4×4 矩阵，每行按 `[a, b, c, d]` 格式打印。
- 矩阵含义：  
  左上角 3×3 为旋转部分，最后一列前三个元素为平移向量（齐次坐标）。  
  **该矩阵可以将源点云（地图）变换到目标点云（雷达扫描）**，即  
  `P_scan ≈ T * P_map`  
  其中 `T = [R t; 0 1]`。

---

## 如何判断结果是否正确

### 1. identity 模式测试
- **期望输出**：单位矩阵
  ```
  [1.0000, 0.0000, 0.0000, 0.0000]
  [0.0000, 1.0000, 0.0000, 0.0000]
  [0.0000, 0.0000, 1.0000, 0.0000]
  [0.0000, 0.0000, 0.0000, 1.0000]
  ```
  允许微小误差（如 0.0001 级别）。Score 应为 0.000000。

### 2. shifted 模式测试
施加已知变换（旋转 R，平移 t）后，ICP 应计算出这个变换的**逆**，即：
`T_icp = [R^T | -R^T * t]`  
因此，你手动施加的变换与打印矩阵的关系为：
- 旋转部分：打印的旋转矩阵应等于你施加旋转的**转置**（因为旋转矩阵的逆等于转置）。  
  例如，施加 `yaw = +1°`（逆时针），则打印矩阵的旋转部分应为顺时针 1°，即左上角出现负的 sin 值（如 `[0.9998, -0.0175; 0.0175, 0.9998]`）。
- 平移部分：打印的平移向量应等于 `-R^T * t`。  
  对于小角度，近似等于 `(-t_x, -t_y)`，即平移取反。

**验证示例**：  
设置 `yaw: 0.0175`（≈1°），`translation_x: 0.1`，`translation_y: -0.05`  
期望矩阵大致为：
```
[ 0.9998, -0.0175, 0,  0.1003 ]
[ 0.0175,  0.9998, 0, -0.0500 ]
[ 0,       0,      1,  0      ]
[ 0,       0,      0,  1      ]
```
如果打印矩阵与此接近，且 Score < 0.001，则证明 ICP 工作正常。

如果打印矩阵与期望偏差较大（例如旋转角度不对、平移正负反了），检查：
- 最近邻匹配阈值是否过小导致错误配对；
- 施加的变换是否过大导致 ICP 收敛到局部极值；
- 地图文件和发布点云的坐标变换是否一致。

---

## 常见问题

### Q1: ICP 输出全是单位阵，即使 shifted 模式下也是。
A: 确认 publisher_node 的日志中前三点坐标是否发生了变化。如果没有变化，说明 YAML 参数未生效（检查缩进和 key 名称）。如果坐标变了但 ICP 仍输出单位阵，可能是 `matchFlag` 已为 true 导致 ICP 未重新计算，可注释掉 `runAlgorithm()` 开头的 `if (matchFlag) return;` 强制每次计算。

### Q2: Score 很大（>0.1）
A: 说明匹配质量差，可能因为阈值太小，大量点未配对，或初始对应关系错误。先确保 identity 模式下 Score=0，再逐步加大变换。

### Q3: 修改了参数但不生效
A: YAML 文件必须严格遵循 ROS 参数格式。确认 key 的层级（节点名/ros__parameters），并用 `colcon build` 重新编译后运行。

---

## 总结
通过 identity 和 small shifted 两项测试，可以充分验证 ICP 核心算法的正确性。一旦通过，即可将 `skidpad_icp` 模块集成到整车控制系统中。