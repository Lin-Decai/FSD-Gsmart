# TODO — 待解决问题

> 最后更新: 2026-05-17

---

## 🔴 TODO 1：仿真下聚类锥桶数仍不足

**现状**：调整后仅检出 14 个锥桶（需 20+ 才能保证 ICP 精度）。

**背景**：FSDS 仿真器 UE4 雷达光线追踪单线程限制，被迫降低 `PointsPerScan`、`RotationsPerSecond` 来防卡顿，导致点云稀疏。YAML 覆盖了 `min_size` 等参数补偿仿真低密度，但可能引入假阳性或仍检出不足。

**待做**：
- [ ] 继续优化 `settings.json` (PointsPerScan / RotationsPerSecond / FOV) 与 `lidar_cone_detector.yaml` (min_size / tolerance / 几何约束) 的参数组合
- [ ] 在仿真器性能限制 (~100k 点/秒) 和聚类检出率之间找到更好的平衡点
- [ ] 注意：所有修改通过 YAML 覆盖实现，不要改动 C++ 源码中的默认值（那是实车用的）

---

## 🔴 TODO 2：确认发车点到起点线的精确距离

**现状**：`check_spawn_distance.py` 脚本测得约 15.0m，与赛规一致。但轨迹仍存在偏移，怀疑距离可能不够精确或存在其他坐标系偏差。

**待做**：
- [ ] 通过更多方式验证距离（例如对比 PCD 中多个参考锥桶的坐标、手动在仿真器中测量等）
- [ ] 确认 `settings.json` 中 `Vehicles.FSCar.X/Y` 是否需要微调来修正发车位置
- [ ] 如确认距离正确但轨迹仍偏移，排查 ICP 变换矩阵精度（锥桶质量、匹配阈值）

---

## 🔴 TODO 3：可视化坐标系 Y 轴方向修复

**现象**：可视化窗口（`skidpad_visualizer`）中蓝色车辆箭头的转向与实际相反——车右转时箭头左转。

**根因**：可视化使用了两个不同坐标系的数据：
- 车辆状态来自 `/estimation/slam/state`（目标坐标系: X=前=北, **Y=左=西**）
- 锥桶来自 `/hesai/cone_positions`（传感器坐标系: X=前, **Y=左**）
- 但 `visualize.py` 中车身坐标系转换的 `delta_y` 计算（`skidpad_track.cpp` 第 130/177 行）写成了 `trajectory_[i].pts.y + py` 而非 `- py`

**待做**：
- [ ] 修复可视化中的 Y 轴方向问题，使显示与实际一致
- [ ] 注意 `skidpad_track.cpp` 的 `+ py` 是之前专门改的，不要动——只修可视化层面的显示
