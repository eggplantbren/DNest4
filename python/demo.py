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


model = Model()
print(list(run(model, 100)))
