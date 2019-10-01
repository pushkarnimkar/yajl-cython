# -*- coding: utf-8 -*-
from distutils.core import setup
from distutils.extension import Extension
from Cython.Build import cythonize

ext_modules = cythonize([
    Extension("addition", ["addition.pyx", "cpp/addition.cpp"], 
              language="c++")
])

setup(name="addition", version="0.0.1", 
      ext_modules=ext_modules)
