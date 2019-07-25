
# Load the data
data = read.table("../../Examples/StraightLine/road.txt")
xs = data[, 1]
ys = data[, 2]

# Size of parameter space
num_params = 3L


# Transform from uniform prior
from_uniform = function(params)
{
    params2 = params
    params2[1] = 1E3*qnorm(params[1])
    params2[2] = 1E3*qnorm(params[2])
    params2[3] = exp(-10.0 + 20.0*params[3])
    return(params2)
}


# Log likelihood function. Argument is a parameter vector where
# the priors are iid Uniform(0, 1). It is your responsibility to
# apply a transformation to produce the prior you want.
log_likelihood = function(params)
{
    params2 = from_uniform(params)
    m = params2[1]
    b = params2[2]
    sigma = params2[3]
    logl = sum(dnorm(ys, mean=m*xs + b, sd=sigma, log=TRUE))
    return(logl)
}

