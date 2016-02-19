#!/usr/bin/env python
# -*- coding: utf-8 -*-
"""
Demonstration of DNest4 on a model where we know the evidence integral.

"""

import dnest4
import numpy as np
from scipy.special import erf

RANGE = 5.0
true_log_z = np.log(0.5*erf(RANGE/np.sqrt(2))/RANGE)
print("ANALYTIC log(Z): {0}".format(true_log_z))


class Model(object):

    def from_prior(self):
        return np.random.uniform(-RANGE, RANGE, size=(1,))

    def perturb(self, coords):
        coords[0] += np.random.randn() * 10**np.random.uniform(-5.0, -0.1)
        coords[0] = (coords[0] + RANGE) % (2*RANGE) - RANGE
        return 0.0

    def log_likelihood(self, coords, const=-0.5*np.log(2*np.pi)):
        return -0.5*coords[0]**2 + const


model = Model()
sampler = dnest4.DNest4Sampler(model,
                               backend=dnest4.backends.CSVBackend(".", sep=" "))
gen = sampler.sample(100, num_steps=200, new_level_interval=10000,
                     num_per_step=100000, thread_steps=200,
                     num_particles=1, lam=5, beta=100, seed=1234)

for i, sample in enumerate(gen):
    if (i + 1) % 10 == 0:
        stats = sampler.postprocess()
        print("Sample {0}: log(Z) = {1} [exact log(Z) = {2}]"
              .format(i + 1, stats["log_Z"], true_log_z))
