# -*- coding: utf-8 -*-
from distutils.core import setup
from distutils.extension import Extension
from Cython.Build import cythonize
from os import path, environ


EXTENSION_NAME = "simple_array"

SOURCE_ROOT = path.dirname(path.dirname(path.dirname(__file__)))

CYTHON_DIR = path.join(SOURCE_ROOT, "cython", EXTENSION_NAME)
CPP_DIR = path.join(SOURCE_ROOT, "cpp", EXTENSION_NAME)

print("CYTHON_DIR => ", CYTHON_DIR)
print("CPP_DIR    => ", CPP_DIR)

CYTHON_SOURCES = [
    path.join(CYTHON_DIR, source_file) 
    for source_file in ["simple_array.pyx"]
]
CPP_SOURCES = [
    path.join(CPP_DIR, source_file) 
    for source_file in ["simple_array.cpp"]
]

SOURCES = CYTHON_SOURCES + CPP_SOURCES

try:
    include_dirs = [path.join(environ["CONDA_PREFIX"], "include")]
except KeyError:
    include_dirs = None

ext_modules = cythonize([
    Extension(EXTENSION_NAME, SOURCES, language="c++",libraries=["yajl"], 
              include_dirs=include_dirs)
])

setup(name="simple_array", version="0.0.1", 
      ext_modules=ext_modules)

