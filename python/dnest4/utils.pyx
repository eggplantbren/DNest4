# -*- coding: utf-8 -*-
__all__ = ["randh", "wrap"]

cimport cython
from libc.math cimport log, sqrt, abs
from libc.stdlib cimport rand, RAND_MAX
import numpy as np
cimport numpy as np

cpdef double randh():
    """
    Generate from the heavy-tailed distribution.
    """
    cdef double a = np.random.randn()
    cdef double r = rand()
    cdef double b = r/RAND_MAX
    cdef double t = a/sqrt(-log(b))
    cdef double n = np.random.randn()
    return pow(10.0, (1.5 - 3*abs(t)))*n

cpdef double wrap(double x, double a, double b):
    assert b > a
    return (x - a)%(b - a) + a

