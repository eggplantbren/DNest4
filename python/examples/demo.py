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
        return np.random.uniform(0.0, RANGE, size=(1,))

    def perturb(self, coords):
        coords[0] += np.random.randn()
        coords[0] = coords[0] % RANGE
        return 0.0

    def log_likelihood(self, coords):
        return -0.5*(np.sum(coords**2) + np.log(2*np.pi))


num_particles = 10
num_steps = 2000

model = Model()
sampler = dnest4.DNest4Sampler(model)

for i, sample in enumerate(sampler.sample(100, num_per_step=5000)):
    print(i)
