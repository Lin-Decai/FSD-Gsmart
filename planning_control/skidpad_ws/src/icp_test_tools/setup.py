from setuptools import setup
import os
from glob import glob

package_name = 'icp_test_tools'

setup(
    name=package_name,
    version='0.0.1',
    packages=[package_name],
    data_files=[
        ('share/ament_index/resource_index/packages', ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
        (os.path.join('share', package_name, 'launch'), glob('launch/*.py')),
        (os.path.join('share', package_name, 'config'), glob('config/*.yaml')),
    ],
    install_requires=['setuptools'],
    zip_safe=True,
    maintainer='you',
    maintainer_email='you@example.com',
    description='ICP test tools',
    license='MIT',
    entry_points={
        'console_scripts': [
            'publisher_node = icp_test_tools.publisher_node:main',
            'printer_node = icp_test_tools.printer_node:main',
        ],
    },
)