from setuptools import setup, find_packages
import os, sys, subprocess

so_extension = '.so'
if os.uname()[0] == 'Darwin': 
    so_extension = '.dylib'
elif os.uname()[0] == 'cygwin':
    so_extension = '.dll'

LIBSO = os.path.join('..','core','libquikgrid_c' + so_extension)

subprocess.call(["cp",LIBSO,"quikgrid/"])

setup(name='quikgrid',
      version='0.5.3a',
      description='Python Quikgrid API',
      packages = find_packages(exclude=['quikgrid.capi_test']),
      zip_safe=False,
      package_data = {'quikgrid':['libquikgrid_c' + so_extension]},
     )
