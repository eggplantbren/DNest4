import dnest4
import numpy as np
from scipy.special import erf

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
num_steps = 1000

model = Model()
samples = []
sample_info = []

for i, sample in enumerate(dnest4.sample(model, 100, num_steps=num_steps,
                                         num_per_step=10000,
                                         num_particles=num_particles)):
    levels = sample["levels"]
    samples.append(sample["samples"])
    sample_info.append(sample["sample_info"])

    if (i + 1) % 500 == 0:
        stats = dnest4.postprocess(levels, np.array(samples),
                                   np.array(sample_info))
        print(stats["log_Z"] - true_log_z)
