import sys
if sys.prefix == '/usr':
    sys.real_prefix = sys.prefix
    sys.prefix = sys.exec_prefix = '/home/limdegcai/GSMART/CODE/skidpad_ws/src/icp_test_tools/install/icp_test_tools'
