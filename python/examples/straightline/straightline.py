#!/usr/bin/env python
# -*- coding: utf-8 -*-
"""
Demonstration of DNest4 in Python using the "StraightLine" example
"""

import dnest4
import numpy as np
import numpy.random as rng
from numba import jitclass, int32

# Load the data
data = np.loadtxt("../../../code/Examples/StraightLine/road.txt")

@jitclass([('dummy', int32)])
class Model(object):
    """
    Specify the model in Python.
    """
    def __init__(self):
        """
        Parameter values *are not* stored inside the class
        """
        pass

    def from_prior(self):
        """
        Unlike in C++, this must *return* a numpy array of parameters.
        """
        m = 1E3*rng.randn()
        b = 1E3*rng.randn()
        sigma = np.exp(-10.0 + 20.0*rng.rand())
        return np.array([m, b, sigma])
    
    def perturb(self, params): 
        """
        Unlike in C++, this takes a numpy array of parameters as input,
        and modifies it in-place. The return value is still logH.
        """
        logH = 0.0
        which = rng.randint(3)

        if which == 0 or which == 1:
            logH -= -0.5*(params[which]/1E3)**2
            params[which] += 1E3*self.randh()
            logH += -0.5*(params[which]/1E3)**2
        else:
            log_sigma = np.log(params[2])
            log_sigma += 20*self.randh()
            # Note the difference between dnest4.wrap in Python and
            # DNest4::wrap in C++. The former *returns* the wrapped value.
            log_sigma = self.wrap(log_sigma, -10.0, 10.0)
            params[2] = np.exp(log_sigma)

        return logH

    def log_likelihood(self, params):
        """
        Gaussian sampling distribution.
        """
        m, b, sigma = params
        var = sigma**2
        return -0.5*data.shape[0]*np.log(2*np.pi*var)\
                - 0.5*np.sum((data[:,1] - (m*data[:,0] + b))**2)/var

    def randh(self):
        """
        Generate from the heavy-tailed distribution.
        """
        a = np.random.randn()
        b = np.random.rand()
        t = a/np.sqrt(-np.log(b))
        n = np.random.randn()
        return 10.0**(1.5 - 3*np.abs(t))*n

    def wrap(self, x, a, b):
        assert b > a
        return (x - a)%(b - a) + a

# Create a model object and a sampler
model = Model()
sampler = dnest4.DNest4Sampler(model,
                               backend=dnest4.backends.CSVBackend(".",
                                                                  sep=" "))

# Set up the sampler. The first argument is max_num_levels
gen = sampler.sample(max_num_levels=30, num_steps=1000, new_level_interval=10000,
                      num_per_step=10000, thread_steps=100,
                      num_particles=5, lam=10, beta=100, seed=1234)

# Do the sampling (one iteration here = one particle save)
for i, sample in enumerate(gen):
    print("# Saved {k} particles.".format(k=(i+1)))

# Run the postprocessing
dnest4.postprocess()

