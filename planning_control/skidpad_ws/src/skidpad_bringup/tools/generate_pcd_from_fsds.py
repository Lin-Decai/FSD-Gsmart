#!/usr/bin/env python3
"""
从 FSDS 仿真器获取锥桶坐标，生成 PCD 地图文件供 ICP 使用。

使用方法:
  1. 启动 FSDS 仿真器 (start-fsds.sh), 加载 Skidpad 地图, 进入"独立窗口游戏"
  2. 运行本脚本: python3 generate_pcd_from_fsds.py
  3. 生成的 PCD 文件保存在 skidpad_icp/config/skidpad.pcd

坐标系转换:
  FSDS 使用 ENU (x=东=前, y=北=左).
  PCD 直接存储目标坐标系 (X=直道=北, Y=左侧=西), loadFiles 不做变换.
"""

import sys
import os
import struct
import math

# 添加 FSDS Python 路径
FSDS_PATH = os.path.expanduser('~/Formula-Student-Driverless-Simulator/python')
sys.path.insert(0, FSDS_PATH)
import fsds


def main():
    print('正在连接 FSDS 仿真器...')
    client = fsds.FSDSClient()
    client.confirmConnection()
    print('连接成功!')

    # 获取锥桶位置
    referee = client.getRefereeState()
    cones = referee.cones

    print(f'获取到 {len(cones)} 个锥桶:')
    color_names = {0: 'Yellow', 1: 'Blue', 2: 'OrangeLarge', 3: 'OrangeSmall'}
    color_counts = {}
    for cone in cones:
        # cone 可能是 dict 或 object，兼容两种格式
        if isinstance(cone, dict):
            color = cone.get('color', -1)
        else:
            color = cone.color
        cname = color_names.get(color, f'Unknown({color})')
        color_counts[cname] = color_counts.get(cname, 0) + 1
    for name, count in color_counts.items():
        print(f'  {name}: {count}')

    # 获取 PlayerStart 位置 (UE4 世界坐标 cm)
    start_pos = referee.initial_position
    if isinstance(start_pos, dict):
        ps_x_cm = start_pos.get('x', 0.0)
        ps_y_cm = start_pos.get('y', 0.0)
    else:
        ps_x_cm = start_pos.x
        ps_y_cm = start_pos.y
    print(f'\nPlayerStart (UE4 cm): x={ps_x_cm:.1f}, y={ps_y_cm:.1f}')

    # 读取 settings.json 获取车辆出生偏移
    import json
    settings_path = os.path.expanduser('~/Formula-Student-Driverless-Simulator/settings.json')
    with open(settings_path) as f:
        settings = json.load(f)
    vehicle = settings['Vehicles']['FSCar']
    veh_x_m = vehicle.get('X', 0.0)
    veh_y_m = vehicle.get('Y', 0.0)
    print(f'车辆偏移 (meters): X={veh_x_m:.2f}, Y={veh_y_m:.2f}')

    # 车的实际出生点 = PlayerStart + 偏移 (m → cm)
    spawn_x_cm = ps_x_cm + veh_x_m * 100.0
    spawn_y_cm = ps_y_cm + veh_y_m * 100.0
    print(f'车辆出生点 (UE4 cm): x={spawn_x_cm:.1f}, y={spawn_y_cm:.1f}')

    # 坐标系转换:
    #   UE4 world (cm, x=north, y=east) → 目标坐标系 (X=直道=北, Y=左侧=西, 原点=车出生点)
    #   PCD 直接保存目标坐标, loadFiles 不再做变换
    points = []
    for cone in cones:
        if isinstance(cone, dict):
            ue4_x = cone.get('x', 0.0)
            ue4_y = cone.get('y', 0.0)
        else:
            ue4_x = cone.x
            ue4_y = cone.y
        # UE4 cm → ENU m (相对车出生点)
        enu_x = (ue4_y - spawn_y_cm) / 100.0   # 右侧(东)
        enu_y = (ue4_x - spawn_x_cm) / 100.0   # 直道(北)
        # 目标坐标系: X=直道=北, Y=左侧=西 (直接存入PCD, loadFiles不做变换)
        tx = enu_y   # 北 → X
        ty = -enu_x  # 西 = -东 → Y
        points.append((tx, ty, 0.0))

    # 写入 PCD 文件 (ASCII 格式便于检查)
    output_dir = os.path.expanduser(
        '~/GSMART/CODE/FSD-Gsmart/planning_control/skidpad_ws/src/skidpad_icp/config')
    os.makedirs(output_dir, exist_ok=True)

    # 保存为 ASCII PCD
    output_path = os.path.join(output_dir, 'skidpad.pcd')
    with open(output_path, 'w') as f:
        f.write('# .PCD v.7 - Point Cloud Data file format\n')
        f.write('VERSION .7\n')
        f.write('FIELDS x y z\n')
        f.write('SIZE 4 4 4\n')
        f.write('TYPE F F F\n')
        f.write('COUNT 1 1 1\n')
        f.write(f'WIDTH {len(points)}\n')
        f.write('HEIGHT 1\n')
        f.write('VIEWPOINT 0 0 0 1 0 0 0\n')
        f.write(f'POINTS {len(points)}\n')
        f.write('DATA ascii\n')
        for x, y, z in points:
            f.write(f'{x:.6f} {y:.6f} {z:.6f}\n')

    # 保存目标坐标系参考文件
    world_path = os.path.join(output_dir, 'skidpad_cones_target.txt')
    with open(world_path, 'w') as f:
        f.write(f'# Cone positions in TARGET frame (X=north=straight, Y=west=left, origin=spawn)\n')
        f.write(f'# spawn UE4 cm: ({spawn_x_cm:.1f}, {spawn_y_cm:.1f})\n')
        f.write('# format: X Y color\n')
        for cone in cones:
            if isinstance(cone, dict):
                ux, uy = cone.get('x', 0.0), cone.get('y', 0.0)
                wc = cone.get('color', -1)
            else:
                ux, uy = cone.x, cone.y
                wc = cone.color
            enu_x = (uy - spawn_y_cm) / 100.0
            enu_y = (ux - spawn_x_cm) / 100.0
            tx = enu_y    # 北→目标X (直道)
            ty = -enu_x   # 西→目标Y (左)
            f.write(f'{tx:.6f} {ty:.6f} {wc}\n')

    print(f'\nPCD 文件已保存: {output_path}')
    print(f'ENU坐标参考文件: {world_path}')
    print(f'共 {len(points)} 个点')
    print('\n请将此 PCD 文件路径配置到 skidpad_detector.yaml 的 path.skidpad_map 中')


if __name__ == '__main__':
    main()
