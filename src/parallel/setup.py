from setuptools import find_packages, setup

package_name = 'parallel'

setup(
    name=package_name,
    version='0.0.1',
    packages=find_packages(exclude=['test']),
    data_files=[
        ('share/ament_index/resource_index/packages',
            ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
        ('share/' + package_name + '/launch',
            ['launch/sift_matching.launch.py']),
    ],
    install_requires=['setuptools'],
    zip_safe=True,
    maintainer='user',
    maintainer_email='user@example.com',
    description='ROS2 package for parallel SIFT feature extraction and FLANN matching',
    license='MIT',
    tests_require=['pytest'],
    entry_points={
        'console_scripts': [
            'sift_publisher = parallel.sift_publisher:main',
            'flann_subscriber = parallel.flann_subscriber:main',
        ],
    },
)