import numpy as np
import rpy2.robjects as ro
from rpy2.robjects import numpy2ri

numpy2ri.activate()

r = ro.r

# Source the R file to load the function
ro.r["source"]("mymodel.R")

num_params = int(ro.globalenv["num_params"])

r_prior_transform = ro.globalenv["prior_transform"]
r_log_likelihood = ro.globalenv["log_likelihood"]

def prior_transform(us):
    return r_prior_transform(us)

def log_likelihood(xs):
    return r_log_likelihood(xs)

def both(us):
    return log_likelihood(prior_transform(us))

