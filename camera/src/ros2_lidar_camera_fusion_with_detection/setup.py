from setuptools import find_packages, setup

package_name = 'ros2_lidar_camera_fusion_with_detection'

setup(
    name=package_name,
    version='1.0.0',
    packages=find_packages(exclude=['test']),
    data_files=[
        ('share/ament_index/resource_index/packages',
            ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),  
        ('share/' + package_name, ['launch/lidar_camera_fusion_with_detection_launch.py']),  
        ('share/' + package_name + '/config', ['config/setup_config1.yaml']),  # Include YAML config file
        ('share/' + package_name + '/config', ['config/setup_config2.yaml']),  # Include YAML config file
    ],
    install_requires=['setuptools','yolo_msgs'],
    zip_safe=True,
    maintainer='AbdullahGM1',
    maintainer_email='agm.musalami@gmail.com',
    description='Lidar and Camera sensors fusion with detection',
    license='MIT',
    tests_require=['pytest'],
    entry_points={
        'console_scripts': [
            'lidar_camera_fusion_with_detection1 = ros2_lidar_camera_fusion_with_detection.lidar_camera_fusion_with_detection1:main',
            'lidar_camera_fusion_with_detection2 = ros2_lidar_camera_fusion_with_detection.lidar_camera_fusion_with_detection2:main',
            'detection1 = ros2_lidar_camera_fusion_with_detection.detection1:main',
            'multi_sensor_fusion_node = ros2_lidar_camera_fusion_with_detection.multi_sensor_fusion_node:main',
        ],
    },
)
