from libcpp.vector cimport vector


# https://stackoverflow.com/questions/28203670/how-to-use-cython-typed-memoryviews-to-accept-strings-from-python
cdef extern from "simple_array.h":
    vector[int]* parse(const unsigned char* buffer, int length)

