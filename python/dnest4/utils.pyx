# -*- coding: utf-8 -*-
__all__ = ["randh", "wrap", "rand", "new_randh", "randn"]

cimport cython
from libc.math cimport cos, log, sqrt, abs, M_PI
from libc.stdlib cimport rand as crand
from libc.stdlib cimport RAND_MAX

cpdef double rand():
    """
    Generate from Uniform(0, 1)
    """
    cdef double r = crand()
    return r/RAND_MAX

cpdef double randn(double mu=0.0, double sigma=1.0):
    """
    Generate from normal distribution N(0, 1) using Box-Muller method
    """
    cdef double x2pi = 2.0*M_PI*rand()
    cdef double g2rad = sqrt(-2.0*log(1.0 - rand()))
    cdef double z = cos(x2pi)*g2rad
    return mu + z*sigma

cpdef double randh():
    """
    Generate from the heavy-tailed distribution.
    """
    cdef double a = randn()
    cdef double b = rand()
    cdef double t = a/sqrt(-log(b))
    cdef double n = randn()
    return pow(10.0, (1.5 - 3*abs(t)))*n

cpdef double wrap(double x, double a, double b):
    assert b > a
    return (x - a)%(b - a) + a

