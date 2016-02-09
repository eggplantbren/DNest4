# Implements the SpikeSlab likelihood.
# Some constants
u = 0.01
v = 0.1
C = log(1.0/sqrt(2*pi))

@doc """
A function.
""" ->
function logsumexp(values::Vector{Float64})
	biggest = maximum(values)
	x = values - biggest
	return log(sum(exp(x))) + biggest
end

@doc """
Evaluate the log likelihood.
""" ->
function log_likelihood(params::Vector{Float64})
	logl1 = length(params)*(C - log(u))
	logl2 = length(params)*(C - log(v))
	for(i in 1:length(params))
		logl1 += -0.5*((params[i] - 0.5 - 0.031)/u)^2
		logl2 += -0.5*((params[i] - 0.5)/v)^2
	end
	logl1 += log(100.0)
	return logsumexp([logl1, logl2])
end

