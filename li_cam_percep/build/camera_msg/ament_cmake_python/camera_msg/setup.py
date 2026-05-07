from setuptools import find_packages
from setuptools import setup

setup(
    name='camera_msg',
    version='0.0.0',
    packages=find_packages(
        include=('camera_msg', 'camera_msg.*')),
)
