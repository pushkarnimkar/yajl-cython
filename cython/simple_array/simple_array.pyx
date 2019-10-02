from simple_array cimport parse
from libcpp.vector cimport vector
import numpy as np


def parse_simple_array(const unsigned char [:] data):
    length = data.shape[0]
    cdef vector[int] *items = parse(&data[0], length)
    print(items.size())
