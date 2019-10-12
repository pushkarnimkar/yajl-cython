from more_complex cimport parse, ComplexReturnType
from libcpp.vector cimport vector
from libcpp.string cimport string
from libcpp.map cimport map
import numpy as np


class SchemaError(Exception):
    pass


def parse_more_complex(const unsigned char [:] data):
    cdef int size = data.shape[0]
    cdef ComplexReturnType* ret = parse(&data[0], size)

    if ret == NULL:
        raise SchemaError

    index = {}
    for key, val in ret[0].index:
        size = len(val)
        arr = np.zeros(size, dtype=np.int32)
        for i in range(size):
            arr[i] = val[i]
        index[key] = arr
        del val

    items = {}
    for key, val in ret[0].items:
        size = len(val)
        arr = np.zeros(size, dtype=np.float64)
        for i in range(size):
            arr[i] = val[i]
        items[key] = arr
        del arr

    return index, items

