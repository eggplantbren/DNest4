# cython: language_level=3, cdivision=True

__all__ = ["rand", "randn", "randt2", "randh", "wrap"]

cimport cython
from libc.math cimport cos, log, sqrt, abs, M_PI
from libc.stdlib cimport rand as crand
from libc.stdlib cimport RAND_MAX
import math

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

cpdef double randt2():
    """
    Generate from t-distribution with 2 degrees of freedom
    """
    return randn()/sqrt(-log(rand()))

cpdef double randh():
    """
    Generate from the heavy-tailed distribution.
    """
    return pow(10.0, (1.5 - 3*abs(randt2())))*randn()

cpdef double wrap(double x, double a, double b):
    assert b > a
    result = my_mod(x - a, b - a) + a
    return result

cpdef my_mod(double y, double x):
    assert(x > 0.0);
    return (y/x - math.floor(y/x))*x;

