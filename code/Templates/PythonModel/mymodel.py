import numpy as np


num_params = 10

def prior_transform(us):
    return us - 0.5

def log_likelihood(xs):
    return -0.5*np.sum((xs/0.01)**2)


def both(us):
    return log_likelihood(prior_transform(us))

