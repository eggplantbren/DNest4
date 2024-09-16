num_params = 10


function prior_transform(us)
    return (us .- 0.5)
end

function log_likelihood(xs)
    return (-num_params*0.5*log(2*pi*0.01^2) - 0.5*sum((xs/0.01).^2))
end

function both(us)
    return log_likelihood(prior_transform(us))
end

