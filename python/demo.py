import numpy as np
from dnest4._dnest4 import run


class Model(object):

    def from_prior(self):
        return np.random.randn(1)

    def perturb(self, coords):
        log_H = 0.5*(coords[0]/10.0) ** 2
        coords[0] += 10 * (10.**(1.5 - 6.*np.random.rand()))*np.random.randn()
        log_H -= 0.5*(coords[0]/10.0) ** 2
        return log_H

    def log_likelihood(self, coords):
        return -0.5*(np.sum(coords**2) + np.log(2*np.pi))


print(list(run(Model(), 1234, 2.7182818284590451, 10, 10000, 100000, 200, 25, 10, 100)))
