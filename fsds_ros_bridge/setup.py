from setuptools import setup

package_name = 'fsds_ros_bridge'

setup(
    name=package_name,
    version='1.0.0',
    packages=[package_name],
    data_files=[
        ('share/ament_index/resource_index/packages',
            ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
    ],
    install_requires=['setuptools'],
    zip_safe=True,
    maintainer='Your Name',
    maintainer_email='your@email.com',
    description='FSDS simulator to ROS 2 bridge: raw lidar, car state, control',
    license='MIT',
    entry_points={
        'console_scripts': [
            'fsds_bridge_node = fsds_ros_bridge.bridge_node:main',
        ],
    },
)
