#!/usr/bin/env python
# -*- coding: utf-8 -*-
"""
Demonstration of DNest4 on a model where we know the evidence integral.

"""

import dnest4
import numpy as np
from scipy.special import erf


class Model(object):

    def __init__(self, ndim=5, rng=5.0):
        self.ndim = ndim
        self.rng = rng

    def analytic_log_Z(self):
        return (
            self.ndim * np.log(erf(self.rng/np.sqrt(2))) -
            self.ndim * np.log(2*self.rng)
        )

    def from_prior(self):
        return np.random.uniform(-self.rng, self.rng, size=(self.ndim,))

    def perturb(self, coords):
        i = np.random.randint(self.ndim)
        coords[i] += np.random.randn() * 10**np.random.uniform(-5.0, -0.1)
        coords[i] = (coords[i] + self.rng) % (2*self.rng) - self.rng
        return 0.0

    def log_likelihood(self, coords, const=-0.5*np.log(2*np.pi)):
        return -0.5*np.sum(coords**2) + self.ndim * const


model = Model()
sampler = dnest4.DNest4Sampler(model,
                               backend=dnest4.backends.CSVBackend(".",
                                                                  sep=" "))
gen = sampler.sample(20, num_steps=1000, new_level_interval=10000,
                     num_per_step=10000, thread_steps=100,
                     num_particles=5, lam=5, beta=100, seed=1234)

for i, sample in enumerate(gen):
    if (i + 1) % 10 == 0:
        stats = sampler.postprocess()
        print("Sample {0}: log(Z) = {1} [exact log(Z) = {2}]"
              .format(i + 1, stats["log_Z"], model.analytic_log_Z()))
