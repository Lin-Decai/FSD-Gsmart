# Skidpad Control

纯跟踪横向控制 + PID 纵向控制（仿真模式）/ 期望速度输出（实车模式）。

## 速度调参

纵向控制采用 PID + 死区 + 低通滤波 + 油门硬限幅。

| 目标速度 | max_throttle | max_acceleration | 效果 |
|----------|-------------|------------------|------|
| 4.0 m/s | 0.30 | 2.5 | 稳定，误差 < 0.3 m/s |
| 6.0 m/s | 0.50 | 3.0 | 稳定 |
| 8.0 m/s | 0.65 | 4.0 | 稳定 |
| 9.5 m/s | 0.85 | 4.0 | 稳定，接近物理极限 |

## 如何调速

**1. 改期望速度** — `config/pure_pursuit_params.yaml`

```yaml
target_speed: 6.0   # 改这里
```

**2. 改最大油门** — `skidpad_control/node.py`

```python
cmd.throttle.data = min(MAX_THROTTLE, output['acceleration'] * 0.15)
```

经验值：`MAX_THROTTLE ≈ 目标速度 × 0.08`。速度越快风阻越大，需要更大油门。

**3. 改 PID 限幅** — `config/pure_pursuit_params.yaml`

```yaml
max_acceleration: 3.0   # 高速启动段误差大，需要更大限幅
```

## 完整参数表

| 参数 | 值 | 位置 | 说明 |
|------|-----|------|------|
| kp | 0.8 | yaml | |
| ki | 0.15 | yaml | |
| kd | 0.2 | yaml | |
| 死区 | ±0.3 m/s | controller.py | 误差小则清零积分防振荡 |
| 积分限幅 | ±0.3 | controller.py | 防积分过冲 |
| 低通 α | 0.85 | controller.py | 加速度平滑 |
| 转向滤波 α | 0.7 | controller.py | 方向盘低通 |
| 最小油门 | 0.03 | node.py | 怠速维持 |

## 仿真 vs 实车

- **仿真**：PID 计算 → 油门/刹车 → FSDS
- **实车**：直接输出期望速度，底层硬件自行控制
