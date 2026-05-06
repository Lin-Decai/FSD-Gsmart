# 单个轨迹点信息（车体坐标系）
geometry_msgs/Point pts          # 坐标 (x 向前, y 向左)
std_msgs/Float64 yaw             # 期望航向角
std_msgs/Float64 curvature       # 路径曲率
std_msgs/Float64 velocity        # 期望速度
std_msgs/Float64 r               # 横摆角速度（预留）
std_msgs/Float64 acc             # 期望加速度（预留）