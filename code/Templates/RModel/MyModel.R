
# Load the data
data = read.table("../../Examples/StraightLine/road.txt")
xs = data[, 1]
ys = data[, 2]

# Size of parameter space
num_params = 3L


# Transform from uniform prior
prior_transform = function(us)
{
    params = us
    params[1] = 1E3*qnorm(us[1])
    params[2] = 1E3*qnorm(us[2])
    params[3] = exp(-10.0 + 20.0*us[3])
    return(params)
}


# Log likelihood function
log_likelihood = function(params)
{
    m = params[1]
    b = params[2]
    sigma = params[3]
    logl = sum(dnorm(ys, mean=m*xs + b, sd=sigma, log=TRUE))
    return(logl)
}


both = function(us)
{
    return (log_likelihood(prior_transform(us)))
}
