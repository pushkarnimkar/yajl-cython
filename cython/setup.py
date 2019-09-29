# -*- coding: utf-8 -*-
from distutils.core import setup
from distutils.extension import Extension
from Cython.Build import cythonize

ext_modules = cythonize([
    Extension("yajl", ["yajl.pyx"], libraries=["yajl"])
])

setup(name="pyyajl", 
      version="0.0.1", 
      packages=["pyyajl"], 
      ext_modules=ext_modules)
