import dnest4
import numpy as np


class Model(object):

    def __len__(self):
        return 1

    def from_prior(self):
        return np.random.randn(1)

    def perturb(self, coords):
        log_H = 0.5*(coords[0]/10.0) ** 2
        coords[0] += 10 * (10.**(1.5 - 6.*np.random.rand()))*np.random.randn()
        log_H -= 0.5*(coords[0]/10.0) ** 2
        return log_H

    def log_likelihood(self, coords):
        return -0.5*(np.sum(coords**2) + np.log(2*np.pi))


num_particles = 10
num_steps = 500

model = Model()
samples = []
sample_info = []

for i, sample in enumerate(dnest4.sample(model, 100, num_steps=num_steps,
                                         num_per_step=10000,
                                         num_particles=num_particles)):
    levels = sample["levels"]
    samples.append(sample["samples"])
    sample_info.append(sample["sample_info"])

    if i > 100:
        dnest4.postprocess(levels, np.array(samples), np.array(sample_info))
