# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Overview

This is an autonomous driving system for a Formula Student Driverless (FSD) vehicle. It comprises:

| Subsystem | Path | Status |
|-----------|------|--------|
| Planning & Control | `planning_control/skidpad/` | Active — ROS 2 colcon workspace |
| FSDS ROS Bridge | `fsds_ros_bridge/` | Active — standalone ROS 2 package |
| Lidar Cone Detector | `lidar/lidar_cone_detector/` | Active — standalone ROS 2 package |
| Camera Perception | `camera/` | Active |
| SLAM & Localization | `slam_localization/` | Placeholder (empty) |

## Build

All packages require ROS 2 Humble and colcon.

```bash
# 1. FSDS ROS Bridge (standalone)
cd fsds_ros_bridge
source /opt/ros/humble/setup.bash
source planning_control/skidpad/install/setup.bash  # for skidpad_msgs
colcon build --symlink-install

# 2. Lidar cone detector (standalone)
cd lidar/lidar_cone_detector
source /opt/ros/humble/setup.bash
colcon build --symlink-install

# 3. Planning & Control workspace
cd planning_control/skidpad
source /opt/ros/humble/setup.bash
colcon build --symlink-install

# Before first run, generate the reference PCD map
python3 src/skidpad_bringup/tools/generate_pcd_from_fsds.py
```

## Run

Source all workspaces before launching:

```bash
source /opt/ros/humble/setup.bash
source lidar/lidar_cone_detector/install/setup.bash
source fsds_ros_bridge/install/setup.bash
source planning_control/skidpad/install/setup.bash

# Simulation (FSDS simulator)
ros2 launch skidpad_bringup skidpad_bringup.launch.py mode:=simulation

# Real vehicle
ros2 launch skidpad_bringup skidpad_bringup.launch.py mode:=real_vehicle

# Visualization
ros2 run skidpad_control skidpad_visualizer
```

## Architecture

The system is a modular pipeline for figure-8 ("skidpad") autonomous driving, supporting both FSDS simulator and real-vehicle modes.

### Topic flow (simulation mode)

```
FSDS API ──[fsds_ros_bridge]──> /lidar_points (PointCloud2, raw lidar)
                             ──> /estimation/slam/state (CarState)
                             ──> /go_cmd (Bool)

/lidar_points ──[lidar_cone_detector]──> /hesai/cone_positions (PoseArray)

/hesai/cone_positions ──[skidpad_icp]──> /transform_matrix (Float64MultiArray)

/estimation/slam/state ────────────────[skidpad_planner]──> /planning/ref_path (Trajectory, body-frame)
                                                          ──> /visual/ref_path (MarkerArray, RViz)

/planning/ref_path ────────────────────[skidpad_control]──> /control_command (ControlCommand)
/go_cmd, /estimation/slam/state
```

### Topic flow (real vehicle mode)

```
[Real Lidar] ──> /lidar_points ──[lidar_cone_detector]──> /hesai/cone_positions
[Real SLAM]  ──> /estimation/slam/state

/hesai/cone_positions ──[skidpad_icp]──> /transform_matrix
/estimation/slam/state ──[planner]──> /planning/ref_path
/planning/ref_path ──────[control]──> /control_command
```

### Coordinate system

All published data uses a unified target coordinate frame: origin at vehicle spawn point, **X = north (straight)**, **Y = west (left)**, units in meters. The FSDS bridge converts from UE4 coordinates (cm, X=north, Y=east) to this target frame.

### Packages

| Package | Path | Lang | Type | Purpose |
|---------|------|------|------|---------|
| `fsds_ros_bridge` | `fsds_ros_bridge/` | Python | `ament_python` | FSDS → ROS 2 bridge: raw lidar, car state, GO signal, control forwarding |
| `lidar_cone_detector` | `lidar/lidar_cone_detector/` | C++ | `ament_cmake` | LiDAR cone detection: ROI→RANSAC→clustering→morphological filter, publishes `/hesai/cone_positions` (PoseArray) |
| `skidpad_msgs` | `planning_control/skidpad/src/` | C++ | `ament_cmake` | Custom ROS 2 messages (CarState, Trajectory, ControlCommand, etc.) |
| `skidpad_icp` | `planning_control/skidpad/src/` | C++ | `ament_cmake` | ICP localization — matches cone positions against reference PCD map, publishes 4x4 transform |
| `skidpad_planner` | `planning_control/skidpad/src/` | C++ | `ament_cmake` | Figure-8 path planner — global trajectory generation + body-frame sampling |
| `skidpad_control` | `planning_control/skidpad/src/` | Python | `ament_python` | Pure pursuit + PID controller |
| `skidpad_bringup` | `planning_control/skidpad/src/` | C++ | `ament_cmake` | Master launch file wiring all nodes |
| `icp_test_tools` | `planning_control/skidpad/src/` | Python | `ament_python` | Test publisher/subscriber for ICP debugging |

### Hardcoded paths

If the workspace is relocated, update these files:

- `src/skidpad_icp/config/skidpad_detector.yaml` — PCD map path (`skidpad_map`)
- `src/skidpad_bringup/tools/generate_pcd_from_fsds.py` — PCD output directory
- `src/icp_test_tools/config/test_params.yaml` — PCD path
- `src/icp_test_tools/icp_test_tools/publisher_node.py` — PCD path default

## Detailed documentation

The `planning_control/skidpad/` directory contains its own `CLAUDE.md` with in-depth explanations of the planner algorithm, ICP matching, controller model, and custom message definitions.
