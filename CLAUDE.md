# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Overview

This is an autonomous driving system for a Formula Student Driverless (FSD) vehicle. It comprises three subsystems:

| Subsystem | Path | Status |
|-----------|------|--------|
| Planning & Control | `planning_control/skidpad_ws/` | Active — ROS 2 colcon workspace |
| Sensor Perception | `sensor_perception/lidar_cone_detector/` | Active — standalone ROS 2 package |
| SLAM & Localization | `slam_localization/` | Placeholder (empty) |

## Build

All ROS 2 packages require ROS 2 Humble and colcon.

**Planning & Control workspace:**

```bash
cd planning_control/skidpad_ws
source /opt/ros/humble/setup.bash
colcon build --symlink-install
source install/setup.bash

# Build a single package
colcon build --symlink-install --packages-select skidpad_icp

# Before first run, generate the reference PCD map
python3 src/skidpad_bringup/tools/generate_pcd_from_fsds.py
```

**Lidar cone detector** (standalone package):

```bash
cd sensor_perception/lidar_cone_detector
source /opt/ros/humble/setup.bash
colcon build --symlink-install
```

## Run

```bash
# Simulation (FSDS simulator)
ros2 launch skidpad_bringup skidpad_bringup.launch.py mode:=simulation

# Real vehicle
ros2 launch skidpad_bringup skidpad_bringup.launch.py mode:=real_vehicle

# Visualization
ros2 run skidpad_control skidpad_visualizer
```

## Architecture

The system is a modular pipeline for figure-8 ("skidpad") autonomous driving, supporting both FSDS simulator and real-vehicle modes. The `simulation`/`real_vehicle` mode parameter controls whether the FSDS bridge is launched (simulation) and how the longitudinal controller models vehicle dynamics.

### Subsystem relationships (conceptual)

```
[Lidar] ──[lidar_cone_detector]──> /lidar_points → cone detection output
                                      ↓ (when integrated)
            [/perception/lidar_cluster] ──[ICP]──> /transform_matrix
                                                        ↓
    [FSDS or real SLAM] ──> /estimation/slam/state ──[planner]──> /planning/ref_path
                                                                        ↓
    /go_cmd ───────────────────────────────────────────────[controller]──> /control_command
```

### Topic flow (simulation mode)

```
FSDS API ──[fsds_bridge]──> /perception/lidar_cluster (PointCloud2)
                         ──> /estimation/slam/state (CarState)
                         ──> /go_cmd (Bool)

/perception/lidar_cluster ──[ICP]──> /transform_matrix (Float64MultiArray)

/estimation/slam/state ─────────────[planner]──> /planning/ref_path (Trajectory, body-frame)
                                   ────────────> /visual/ref_path (MarkerArray, RViz)

/planning/ref_path ─────────────────────────────[control]──> /control_command (ControlCommand)
/go_cmd, /estimation/slam/state
```

### Coordinate system

All published data uses a unified target coordinate frame: origin at vehicle spawn point, **X = north (straight)**, **Y = west (left)**, units in meters. The FSDS bridge converts from UE4 coordinates (cm, X=north, Y=east) to this target frame.

### Packages (planning_control/skidpad_ws/src/)

| Package | Lang | Type | Purpose |
|---------|------|------|---------|
| `skidpad_msgs` | C++ | `ament_cmake` | Custom ROS 2 messages (CarState, Trajectory, ControlCommand, etc.) |
| `skidpad_icp` | C++ | `ament_cmake` | ICP localization — matches cone cluster against reference PCD map, publishes 4x4 world-to-vehicle transform at 50 Hz |
| `skidpad_planner` | C++ | `ament_cmake` | Figure-8 path planner — generates global trajectory once, then samples body-frame reference path at ~100 Hz |
| `skidpad_control` | Python | `ament_python` | Pure pursuit + PID controller at 20 Hz — lateral lookahead steering, longitudinal speed control with end-of-trajectory deceleration |
| `skidpad_bringup` | C++ | `ament_cmake` | Master launch file wiring all nodes |
| `fsds_bridge` | Python | `ament_python` | FSDS simulator ↔ ROS bridge (simulation only) |
| `icp_test_tools` | Python | `ament_python` | Test publisher/subscriber for ICP debugging |

### Lidar cone detector (`sensor_perception/lidar_cone_detector/`)

A standalone ROS 2 C++ node that processes raw LiDAR point clouds to detect traffic cones. Uses a pipeline of: ROI filtering → RANSAC ground removal → Euclidean clustering → morphological filtering. Publishes cone positions as `PoseArray` on `/hesai/cone_positions` and intermediate processing stages as `PointCloud2` topics for debugging.

### Hardcoded paths

If the workspace is relocated, update these files (or use sed across the workspace):

- `src/skidpad_icp/config/skidpad_detector.yaml` — PCD map path (`skidpad_map`)
- `src/skidpad_bringup/tools/generate_pcd_from_fsds.py` — PCD output directory
- `src/icp_test_tools/config/test_params.yaml` — PCD path
- `src/icp_test_tools/icp_test_tools/publisher_node.py` — PCD path default

## Detailed documentation

The `planning_control/skidpad_ws/` directory contains its own `CLAUDE.md` with in-depth explanations of the planner algorithm, ICP matching, controller model, and custom message definitions.
