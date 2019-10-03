from simple_array cimport parse
from libcpp.vector cimport vector
import numpy as np


def parse_simple_array(const unsigned char [:] data):
    length = data.shape[0]
    cdef vector[int] *items_ = parse(&data[0], length)

    # this line gives more clarity about cythons interpretation of pointers
    cdef vector[int] items = items_[0]

    cdef int size = items.size()
    arr = np.zeros(size, dtype=np.int32)
    for i in range(size):
        arr[i] = items[i]

    del items_
    return arr

