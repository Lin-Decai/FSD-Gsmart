from setuptools import setup

package_name = 'camera_node'

setup(
    name=package_name,
    version='0.0.0',
    packages=[package_name],

    data_files=[
        ('share/ament_index/resource_index/packages',
            ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
    ],
    install_requires=['setuptools'],  
    zip_safe=True,
    maintainer='your_name',
    maintainer_email='your_email@example.com',
    description='ROS2 Camera Node',
    license='Apache License 2.0',
    tests_require=['pytest'],
    entry_points={
        'console_scripts': [
            'camera_node_1 = camera_node.camera_node_1:main',
            'camera_node_2 = camera_node.camera_node_2:main',
            'camera_node_1_pro = camera_node.camera_node_1_pro:main',
            'camera_node_2_pro = camera_node.camera_node_2_pro:main',
            'camera_node_yolo_1 = camera_node.camera_node_yolo_1:main',
            'camera_node_yolo_2 = camera_node.camera_node_yolo_2:main',
            'camera_info_1 = camera_node.camera_info_1:main',
            'camera_info_2 = camera_node.camera_info_2:main',
            'camera_node_1_raw = camera_node.camera_node_1_raw:main',
            'camera_node_2_raw = camera_node.camera_node_2_raw:main'
            
        ],
    },
)
