# 轨迹消息，由一系列轨迹点组成
std_msgs/Header header

# 轨迹点数组
skidpad_msgs/TrajectoryPoint[] trajectory

# 当前车辆位置到全局轨迹终点的弧长距离（米）
std_msgs/Float64 remaining_distance