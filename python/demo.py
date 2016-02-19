import numpy as np
from scipy.special import erf
import matplotlib.pyplot as pl

import dnest4
from dnest4.backends import CSVBackend

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
sampler = dnest4.DNest4Sampler(
    model,
    # backend=CSVBackend("demo")
)

for i, sample in enumerate(sampler.sample(100, num_steps=num_steps,
                                          num_per_step=10000,
                                          num_particles=num_particles)):
    if (i + 1) % 20 == 0:
        sampler.postprocess(plot=True)
        input()
