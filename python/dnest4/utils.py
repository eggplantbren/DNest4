# -*- coding: utf-8 -*-
__all__ = ["randh", "wrap"]

import numpy as np
import numpy.random as rng

def randh(N=1):
    """
    Generate from the heavy-tailed distribution.
    """
    if N==1:
	    return 10.0**(1.5 - 3*np.abs(rng.randn()/np.sqrt(-np.log(rng.rand()))))*rng.randn()
    return 10.0**(1.5 - 3*np.abs(rng.randn(N)/np.sqrt(-np.log(rng.rand(N)))))*rng.randn(N)


def wrap(x, a, b):
    assert b > a
    return (x - a)%(b - a) + a

