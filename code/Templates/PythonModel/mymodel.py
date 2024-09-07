import numpy as np


num_params = 10

def prior_transform(us):
    return us - 0.5

def log_likelihood(params):
    return -num_params*0.5*np.log(2*np.pi*0.01**2)-0.5*np.sum((params/0.01)**2)


def both(us):
    return log_likelihood(prior_transform(us))

