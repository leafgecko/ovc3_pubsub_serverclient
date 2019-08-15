#!/usr/bin/env python

from distutils.core import setup
from catkin_pkg.python_setup import generate_distutils_setup

setup_args = generate_distutils_setup(

    # State your package directories within /src here
    packages = ['ovc3'],

    # Script locations
    scripts = ['src/pubsub_both_ovc'],

    # root/src, basically
    package_dir = {'': 'src'},

    # Your Python dependencies (eg. 'serial')
    install_requires = ['std_msgs', 'sensor_msgs']
)

setup(**setup_args)
