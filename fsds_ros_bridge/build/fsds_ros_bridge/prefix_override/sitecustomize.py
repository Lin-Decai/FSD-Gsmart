import sys
if sys.prefix == '/usr':
    sys.real_prefix = sys.prefix
    sys.prefix = sys.exec_prefix = '/home/limdegcai/GSMART/CODE/FSD-Gsmart/fsds_ros_bridge/install/fsds_ros_bridge'
