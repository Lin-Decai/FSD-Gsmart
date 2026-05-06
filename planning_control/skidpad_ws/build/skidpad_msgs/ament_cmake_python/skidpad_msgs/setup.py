from setuptools import find_packages
from setuptools import setup

setup(
    name='skidpad_msgs',
    version='1.0.0',
    packages=find_packages(
        include=('skidpad_msgs', 'skidpad_msgs.*')),
)
