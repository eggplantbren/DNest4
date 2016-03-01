import numpy as np
import numpy.random as rng

__all__ = "RJObject"


class RJObject:
    """
    An RJObject is a collection of N objects where N is unknown
    and the objects' properties may have a hierarchically-specified
    prior distribution.
    """

    def __init__(self, N_max, dims, conditional_prior):
        """
        N_max (default=10): The maximum number of components allowed
        dims = dimensionality of the parameter space of a component
        conditional_prior = ConditionalPrior object
        """
        self.N_max = N_max
        self.dims = dims
        self.N = 0
        self.components = np.empty(self.N_max, dims)
        self.conditional_prior = conditional_prior

    def from_prior(self):
        """
        Generate hyperparameters and components from the prior
        """
        self.components = rng.rand(self.N_max, self.dims)
        self.conditional_prior.from_prior()
        self.components[0:self.N, :] = rng.rand(self.N, self.dims)

    def perturb(self):
        """
        Metropolis-Hastings proposal
        """

        # Choose a proposal type
        choice = rng.rand_int(5)


    def perturb_N(self):
        """
        Propose a new value for N using birth-death
        """
        pass

