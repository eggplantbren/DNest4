
num_params = 10L

prior_transform = function(us)
{
    return(us - 0.5)
}

log_likelihood = function(xs)
{
    return(-0.5*sum((xs/0.01)^2))
}
