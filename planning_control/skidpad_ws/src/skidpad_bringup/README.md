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

| 节点 | 包 | 模式 |
|------|-----|------|
| fsds_bridge | fsds_bridge | 仅仿真 |
| skidpad_detector_node | skidpad_icp | 全部 |
| path_generator | skidpad_planner | 全部 |
| pure_pursuit_controller | skidpad_control | 全部 |

## 仿真 vs 实车

仿真通过 `fsds_bridge` 提供车辆位姿、锥桶和GO信号。实车由外部SLAM/感知模块提供，话题名称统一。
