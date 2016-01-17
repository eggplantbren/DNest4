@doc """
Evaluate the log likelihood.
""" ->
function log_likelihood(params::Vector{Float64})
	return -0.5*sum((params - 0.5).^2)/0.01^2 - 0.5*length(params)*log(2*pi*0.01^2)
end

