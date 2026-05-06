# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Build

This is a ROS 2 (Humble) colcon workspace.

```bash
# Source ROS 2 first, then build
source /opt/ros/humble/setup.bash
colcon build --symlink-install

# Build a single package
colcon build --symlink-install --packages-select skidpad_icp

# After building, source the workspace
source install/setup.bash
```

## Run

```bash
# Simulation mode (FSDS)
ros2 launch skidpad_bringup skidpad_bringup.launch.py mode:=simulation

# Real vehicle mode
ros2 launch skidpad_bringup skidpad_bringup.launch.py mode:=real_vehicle
```

## Architecture

The system is a 4-node pipeline for autonomous figure-8 ("skidpad") driving, supporting both FSDS simulator and real-vehicle modes.

### 1. ICP localization — `skidpad_icp` (C++, timer-driven at 50 Hz)

- **Input**: cone cluster point cloud (`/perception/lidar_cluster`)
- **Output**: `Float64MultiArray` on `/transform_matrix` (column-major 4x4 world-to-vehicle transform)
- **How**: Loads a reference PCD map, nearest-neighbor matching against the loaded map, then PCL ICP alignment. Publishes the transform matrix only when the match flag is set.
- **Config**: `src/skidpad_icp/config/skidpad_detector.yaml` — PCD path, distance thresholds, ICP convergence params.

### 2. Path planner — `skidpad_planner` (C++, own-rate loop at ~100 Hz via `rclcpp::Rate` + `spin_some`)

- **Input**: `CarState` on `/estimation/slam/state`, transform matrix from ICP.
- **Output**: `Trajectory` on `/planning/ref_path` (body-frame coordinates), `MarkerArray` on `/visual/ref_path` (for RViz).
- **How**:
  - On first receipt of the ICP transform matrix, generates a **global** figure-8 trajectory once: straight → right circle (2 turns) → left circle (2 turns) → straight. Applies the ICP transform to place it in world coordinates.
  - Each cycle: finds the nearest point on the global trajectory from the vehicle's current position (search window: +400 points), computes remaining arc length to end, then samples **N** points ahead using velocity-based step sizing (`s += v * dt`). Each sampled point is converted from world to **body-frame** coordinates (translation + rotation by yaw).
  - Velocity at each point is capped by lateral acceleration limit (`max_lat_acc / curvature`).
- **Config**: `src/skidpad_planner/config/skidpad_planner.yaml` — `desire_vel`, `car_length`, `circle_radius` (9.125 m), `forward_distance`, `interval`, `N`, `dt`, `max_lat_acc`, `initial_velocity`.

### 3. Controller — `skidpad_control` (Python, timer-driven at 20 Hz via `dt=0.05`)

- **Input**: reference trajectory from planner, vehicle speed from `CarState` (on `/estimation/slam/state`), GO signal (`/go_cmd`).
- **Output**: `ControlCommand` on `/control_command` (throttle [-1,1], steering [-1,1]). Publishes `Float64`(1000.0) on `/as_finish` when stopped at trajectory end.
- **How**:
  - **Lateral**: pure pursuit — speed-dependent lookahead distance (`Ld = k*|v| + c`), interpolates lookahead point along the trajectory, computes curvature as `2*y/L²`, steering angle = `atan(curvature * wheel_base)`.
  - **Longitudinal**: PID controller on speed error with low-pass filter smoothing (`alpha=0.7`) and integral anti-windup (clamped to [-2, 2]). End-of-trajectory deceleration: when remaining distance < 10 m, target speed ramps down as `sqrt(2*a*d_remaining)`.
- **Config**: `src/skidpad_control/config/pure_pursuit_params.yaml`.

### 4. FSDS bridge — `fsds_bridge` (Python, launched only in simulation mode)

- Publishes `CarState` on `/estimation/slam/state` (100 Hz), cone `PointCloud2` on `/perception/lidar_cluster` (10 Hz), `Bool` GO signal on `/go_cmd`.
- Subscribes to `ControlCommand` on `/control_command` and forwards it to the FSDS simulator.
- Reads vehicle spawn offset from `~/Formula-Student-Driverless-Simulator/settings.json`.
- Coordinate transform: UE4 cm → ENU m → target frame (X=north=straight, Y=west=left).

### Simulation vs. real vehicle mode

The `mode` parameter (`simulation` or `real_vehicle`) controls:

| Aspect | Simulation | Real vehicle |
|--------|-----------|--------------|
| ICP | Same in both modes | Same |
| Planner | Receives `simulation: true` param (topic name is configurable but defaults to same `/estimation/slam/state`) | `simulation: false` |
| Control | Receives `mode: simulation` param (enables vehicle dynamics modeling params: mass, drag, rolling resistance, etc.) | `mode: real_vehicle` (simpler PID-only model) |
| Bridge | Launched | Not launched |

Key insight: **both modes use the same topic names** (`/estimation/slam/state`, `/go_cmd`, `/control_command`). In simulation, the bridge translates FSDS API calls to these ROS topics; in real-vehicle mode, external perception/SLAM nodes publish to them directly. The control node uses the `mode` parameter only for the longitudinal dynamics model (simulation mode includes torque computation with drag, rolling resistance, drivetrain efficiency).

### Package table

| Package | Lang | Type | Purpose |
|---------|------|------|---------|
| `skidpad_msgs` | C++ | `ament_cmake` | Custom ROS 2 messages |
| `skidpad_icp` | C++ | `ament_cmake` | ICP localization node |
| `skidpad_planner` | C++ | `ament_cmake` | Figure-8 path planner |
| `skidpad_control` | Python | `ament_python` | Pure pursuit controller |
| `skidpad_bringup` | C++ | `ament_cmake` | Master launch file |
| `fsds_bridge` | Python | `ament_python` | FSDS simulator ↔ ROS bridge |
| `icp_test_tools` | Python | `ament_python` | Test publisher/subscriber for ICP debugging |

### Topic flow

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

### Custom messages (`skidpad_msgs/msg/`)

- **`CarState`**: header + `Pose2D car_state` (x, y, theta) + `CarStateDt car_state_dt` (body-frame velocity: `dx/dt`, `dy/dt`).
- **`Trajectory`**: header + `TrajectoryPoint[] trajectory` + `Float64 remaining_distance`.
- **`TrajectoryPoint`**: `Point pts` (x, y), `Float64 yaw`, `Float64 curvature`, `Float64 velocity`, `Float64 acc`, `Float64 r`.
- **`ControlCommand`**: header + `Float64 throttle` [-1, 1] (negative = brake) + `Float64 steering_angle` [-1, 1].

### Planner algorithm detail

The planner runs a self-paced loop in `main.cpp` using `rclcpp::Rate` + `spin_some` (not a ROS timer). The global trajectory is generated once on first ICP receipt. Each iteration:
1. Find closest point on global trajectory to vehicle (search from last index, up to +400 points ahead)
2. Compute remaining arc length to trajectory end
3. Sample N points ahead using `s += current_velocity * dt` stepping
4. Convert each point from world to body frame: `x_body = dx*cos(ψ) + dy*sin(ψ)`, `y_body = dy*cos(ψ) - dx*sin(ψ)`
5. Cap point velocity by `sqrt(max_lat_acc / curvature)`
6. Publish body-frame trajectory + RViz markers colored by velocity
