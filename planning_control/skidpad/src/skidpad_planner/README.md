# skidpad_planner - 路径规划

生成8字绕环全局轨迹，并实时采样局部轨迹发给控制器。

## 坐标系

使用**目标坐标系**：X=北(直道), Y=西(左侧), 原点=车出生点。

## 全局轨迹 (`genTraj`)

```
直道1 (车后2m → 右圈入口 16.88m)
  → 右圈2圈 (顺时针, 中心 x=16.88, y=-9.125)
  → 左圈2圈 (逆时针, 中心 x=16.88, y=+9.125)
  → 直道2 (驶出15m)
```

生成后应用 ICP 变换矩阵对齐到真实世界。

## 局部规划 (`CalculateTraj`)

1. 在全局轨迹上找离车最近的点 (搜索窗口 400 点)
2. 从最近点向前采样 N=40 个点，步长 = v × dt
3. 每个点从全局坐标旋转到车身坐标系 (X=前, Y=左)
4. 弯道限速: v_max = sqrt(max_lat_acc / curvature)

## 数据流

```
/estimation/slam/state (CarState) ─┐
/transform_matrix (Float64) ───────┤
                                   └→ [Planner] → /planning/ref_path (Trajectory)
                                                 → /visual/global_path (MarkerArray)
```

## 关键参数

| 参数 | 默认值 | 说明 |
|------|--------|------|
| forward_distance | 15.0 | 车头距起点线距离 (m) |
| car_length | 1.88 | 车辆参考点到车头距离 (m) |
| circle_radius | 9.125 | 八字圆半径 (m) |
| N | 40 | 局部轨迹采样点数 |
| dt | 0.04 | 采样时间步长 (s) |
| max_lat_acc | 3.0 | 最大侧向加速度 (m/s²) |
| node_rate | 100 | 规划频率 (Hz) |
