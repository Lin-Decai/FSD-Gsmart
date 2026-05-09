# skidpad_bringup - 启动文件

统一启动所有规控节点。

## 使用

```bash
# 仿真模式 (FSDS)
ros2 launch skidpad_bringup skidpad_bringup.launch.py mode:=simulation

# 实车模式
ros2 launch skidpad_bringup skidpad_bringup.launch.py mode:=real_vehicle
```

## 启动的节点

| 节点 | 包 | 模式 | 说明 |
|------|-----|------|------|
| fsds_bridge | fsds_ros_bridge | 仅仿真 | 发布 /lidar_points (原始雷达), /estimation/slam/state, /go_cmd |
| hesai_processor | lidar_cone_detector | 全部 | 雷达锥桶检测: /lidar_points → /hesai/cone_positions |
| skidpad_detector_node | skidpad_icp | 全部 | ICP 配准: /hesai/cone_positions → /transform_matrix |
| path_generator | skidpad_planner | 全部 | 路径规划: → /planning/ref_path |
| pure_pursuit_controller | skidpad_control | 全部 | 纯跟踪控制: → /control_command |

## 仿真 vs 实车

仿真通过 `fsds_ros_bridge` 提供原始雷达点云、车辆位姿和 GO 信号。
实车由真实雷达/SLAM 提供，话题名称统一：
- /lidar_points — 原始雷达点云
- /estimation/slam/state — 车辆状态
- /go_cmd — 启动信号
