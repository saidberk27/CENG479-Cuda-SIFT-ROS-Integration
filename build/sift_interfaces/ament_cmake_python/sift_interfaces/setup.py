from setuptools import find_packages
from setuptools import setup

setup(
    name='sift_interfaces',
    version='0.0.1',
    packages=find_packages(
        include=('sift_interfaces', 'sift_interfaces.*')),
)
