# -*- coding: utf-8 -*-
__all__ = ["randh"]

import numpy as np
import numpy.random as rng

def randh(N=1):
    """
    Generate from the heavy-tailed distribution.
    If N=1, returns a float64
    If N>1, returns a numpy array
    """
    assert N >= 1

    t = rng.randn(N)/np.sqrt((rng.randn(N)**2 + rng.randn(N)**2)/2)
    x = 10.0**(1.5 - 3*np.abs(t))*rng.randn(N)

    if len(x) == 1:
        return x[0]
    return x

