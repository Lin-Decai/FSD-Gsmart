import sys
if sys.prefix == '/usr':
    sys.real_prefix = sys.prefix
    sys.prefix = sys.exec_prefix = '/home/limdegcai/GSMART/CODE/FSD-Gsmart/planning_control/skidpad_ws/install/skidpad_control'
