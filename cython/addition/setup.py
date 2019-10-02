# -*- coding: utf-8 -*-
from distutils.core import setup
from distutils.extension import Extension
from Cython.Build import cythonize
from os import path
 

EXTENSION_NAME = "addition"

SOURCE_ROOT = path.dirname(path.dirname(path.dirname(__file__)))

CYTHON_DIR = path.join(SOURCE_ROOT, "cython", EXTENSION_NAME)
CPP_DIR = path.join(SOURCE_ROOT, "cpp", EXTENSION_NAME)

print("CYTHON_DIR => ", CYTHON_DIR)
print("CPP_DIR    => ", CPP_DIR)

CYTHON_SOURCES = [
    path.join(CYTHON_DIR, source_file) 
    for source_file in ["addition.pyx"]
]
CPP_SOURCES = [
    path.join(CPP_DIR, source_file) 
    for source_file in ["addition.cpp"]
]

SOURCES = CYTHON_SOURCES + CPP_SOURCES


ext_modules = cythonize([
    Extension(EXTENSION_NAME, SOURCES, language="c++")
])

setup(name="addition", version="0.0.1", 
      ext_modules=ext_modules)
