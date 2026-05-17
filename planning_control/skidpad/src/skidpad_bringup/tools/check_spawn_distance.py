#!/usr/bin/env python3
"""确认仿真器中发车点到起点线的距离"""
import sys, os, json
sys.path.insert(0, os.path.expanduser('~/Formula-Student-Driverless-Simulator/python'))
import fsds

client = fsds.FSDSClient()
client.confirmConnection()
print('已连接 FSDS')

ref = client.getRefereeState()
ip = ref.initial_position
if isinstance(ip, dict):
    ps_x_cm, ps_y_cm = ip.get('x', 0.0), ip.get('y', 0.0)
else:
    ps_x_cm, ps_y_cm = ip.x, ip.y

settings_path = os.path.expanduser('~/Formula-Student-Driverless-Simulator/settings.json')
with open(settings_path) as f:
    s = json.load(f)
v = s['Vehicles']['FSCar']
veh_x_m = v.get('X', 0.0)
veh_y_m = v.get('Y', 0.0)

spawn_x_cm = ps_x_cm + veh_x_m * 100.0
spawn_y_cm = ps_y_cm + veh_y_m * 100.0
print(f'PlayerStart (UE4 cm):  x={ps_x_cm:.1f}, y={ps_y_cm:.1f}')
print(f'车辆偏移 (m):          X={veh_x_m:.4f}, Y={veh_y_m:.4f}')
print(f'车实际出生点 (UE4 cm): x={spawn_x_cm:.1f}, y={spawn_y_cm:.1f}')

# 转换到目标坐标
def to_target(ue4_x_cm, ue4_y_cm):
    enu_x = (ue4_y_cm - spawn_y_cm) / 100.0
    enu_y = (ue4_x_cm - spawn_x_cm) / 100.0
    return (enu_y, -enu_x)

# 找所有锥桶，按X排序
color_map = {0: 'Yellow', 1: 'Blue', 2: 'OrangeLarge', 3: 'OrangeSmall'}
cones_target = []
for cone in ref.cones:
    if isinstance(cone, dict):
        ux, uy, c = cone.get('x',0), cone.get('y',0), cone.get('color',-1)
    else:
        ux, uy, c = cone.x, cone.y, cone.color
    tx, ty = to_target(ux, uy)
    cones_target.append((tx, ty, c))

cones_target.sort()

# 找右圈入口锥桶 (X≈15m, Y≈±1.75m)
print()
print('X≈15m附近的锥桶 (右圈入口/起点线):')
for tx, ty, c in cones_target:
    if 13 < tx < 18:
        color = color_map.get(c, str(c))
        print(f'  X={tx:.3f}, Y={ty:7.3f}  ({color})')

# 估算起点线距离：取Y≈±1.75、X最小的那对Blue/Yellow锥桶
print()
print('=== 分析 ===')
blue_yellow = [(tx, ty, c) for tx, ty, c in cones_target if c in (0, 1)]
blue_yellow.sort()
if blue_yellow:
    nearest = blue_yellow[0]
    print(f'最近的赛道锥桶: X={nearest[0]:.3f}m (距发车点{nearest[0]:.3f}m)')

# 右圈入口第一对锥桶
entrance = [(tx, ty, c) for tx, ty, c in cones_target if 13 < tx < 17 and abs(abs(ty)-1.75) < 0.5]
if entrance:
    avg_x = sum(tx for tx, _, _ in entrance) / len(entrance)
    print(f'右圈入口锥桶平均X: {avg_x:.3f}m')
    print(f'发车点到起点线距离 ≈ {avg_x:.3f}m')
