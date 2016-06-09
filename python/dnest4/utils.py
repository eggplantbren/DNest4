# -*- coding: utf-8 -*-
__all__ = ["randh", "wrap"]

import numpy as np
import numpy.random as rng

def randh():
    """
    Generate from the heavy-tailed distribution.
    """
    return 10.0**(1.5 - 3*np.abs(rng.randn()/np.sqrt(-np.log(rng.rand()))))*rng.randn()

def wrap(x, a, b):
    assert b > a
    return (x - a)%(b - a) + a

