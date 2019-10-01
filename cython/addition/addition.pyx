from addition cimport addition
from libc.stdlib cimport malloc, free


def summation(vals):
    cdef int length = len(vals)
    cdef int* data = <int*>malloc(length * sizeof(int))
    
    for i in range(length):
        data[i] = vals[i]
        
    val = addition(data, length)
    free(data)
    return val
