from libcpp.vector cimport vector
from libcpp.map cimport map as cpp_map
from libcpp.string cimport string


cdef extern from "complex_object.h":

    ctypedef struct ComplexReturnType:
        cpp_map[string, vector[int]] index
        cpp_map[string, vector[float]] items

    ComplexReturnType* parse(const unsigned char* buffer, int length)

