from matplotlib import rcParams
rcParams["backend"] = "MacOSX"

import numpy as np
from scipy.special import erf

import dnest4
from dnest4.analysis import subsample_particles
from dnest4 import deprecated

RANGE = 5.0
true_log_z = np.log(0.5*erf(RANGE/np.sqrt(2))/RANGE)
print("ANALYTIC log(Z): {0}".format(true_log_z))


class Model(object):

    def __len__(self):
        return 1

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
samples = []
sample_info = []

for i, sample in enumerate(dnest4.sample(model, 100, num_steps=num_steps,
                                         num_per_step=10000,
                                         num_particles=num_particles)):
    levels = sample["levels"]
    samples.append(sample["samples"])
    sample_info.append(sample["sample_info"])

    if (i + 1) % 20 == 0:
        s, si = subsample_particles(
            np.array(samples), np.array(sample_info)
        )
        stats = dnest4.postprocess(levels, s, si,
                                   # perturb=4,
                                   # compression_scatter=0.01,
                                   resample=1)
        print(stats["log_Z"], true_log_z)
        # print(stats)

        deprecated.postprocess(loaded=[
            np.array([row.tolist() for row in levels], dtype=float),
            np.array([row.tolist() for row in si], dtype=float),
            s,
        ], plot=False)
        # assert 0
