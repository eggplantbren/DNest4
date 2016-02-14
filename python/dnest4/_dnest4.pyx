# distutils: language = c++
from __future__ import division

cimport cython
from libcpp.vector cimport vector
from cython.operator cimport dereference

import time
import numpy as np
cimport numpy as np
np.import_array()

DTYPE = np.float64
ctypedef np.float64_t DTYPE_t


cdef extern from "DNest4.h" namespace "DNest4":

    cdef cppclass Options:
        Options ()
        Options (
            unsigned int num_particles,
            unsigned int new_level_interval,
            unsigned int save_interval,
            unsigned int thread_steps,
            unsigned int max_num_levels,
            double lam,
            double beta,
            unsigned int max_num_saves
        )

    cdef cppclass Sampler[T]:
        Sampler()
        Sampler(unsigned int num_threads, double compression,
                const Options options, unsigned save_to_disk)

        # Setup, running, etc.
        void initialise(unsigned int first_seed)
        void run()
        void increase_max_num_saves(unsigned int increment)

        # Interface.
        int size()
        T* particle(unsigned int i)
        vector[Level] get_levels()

    cdef cppclass LikelihoodType:
        double get_value()
        double get_tiebreaker()

    cdef cppclass Level:
        Level()
        LikelihoodType get_log_likelihood()
        unsigned int get_visits()
        unsigned int get_exceeds()
        unsigned int get_accepts()
        unsigned int get_tries()
        double get_log_X()


cdef extern from "PyModel.h":

    cdef cppclass PyModel:
        void set_py_self (object py_self)
        object get_py_self ()
        int get_exception ()
        object get_npy_coords ()


def run(
    self,

    int num_steps=-1,
    unsigned int num_per_step=10000,

    # sampler args
    unsigned int num_particles=1,
    unsigned int new_level_interval=10000,
    unsigned int thread_steps=200,
    unsigned int max_num_levels=100,

    double lam=10.0,
    double beta=100.0,

    # "command line" arguments
    seed=None,
    double compression=np.exp(1.0),
):
    # Check the model.
    if not hasattr(self, "from_prior") or not callable(self.from_prior):
        raise ValueError("DNest4 models must have a callable 'from_prior' method")
    if not hasattr(self, "perturb") or not callable(self.perturb):
        raise ValueError("DNest4 models must have a callable 'perturb' method")
    if not hasattr(self, "log_likelihood") or not callable(self.log_likelihood):
        raise ValueError("DNest4 models must have a callable 'log_likelihood' method")

    # Set up the options.
    if (num_per_step <= 0 or num_particles <= 0 or new_level_interval <= 0
            or max_num_levels <= 0):
        raise ValueError("'num_per_step', 'num_particles', "
                         "'new_level_interval', and "
                         "'max_num_levels' must all be positive")
    if lam <= 0.0 or beta < 0.0:
        raise ValueError("'lam' and 'beta' must be non-negative")
    cdef Options options = Options(
        num_particles, new_level_interval, num_per_step, thread_steps,
        max_num_levels, lam, beta, 1
    )

    # Declarations.
    cdef int i, j, n, error
    cdef Sampler[PyModel] sampler = Sampler[PyModel](1, compression, options, 0)
    cdef PyModel* particle
    cdef vector[Level] levels
    cdef Level level

    # Initialize the particles.
    n = sampler.size()
    for j in range(n):
        particle = sampler.particle(j)
        particle.set_py_self(self)
        error = particle.get_exception()
        if error != 0:
            raise DNest4Error(error)

    # Initialize the sampler.
    if seed is None:
        seed = time.time()
    cdef unsigned int seed_ = int(abs(seed))
    sampler.initialise(seed_)
    n = sampler.size()
    for j in range(n):
        particle = sampler.particle(j)
        error = particle.get_exception()
        if error != 0:
            raise DNest4Error(error)

    i = 0
    while num_steps < 0 or i < num_steps:
        sampler.run()

        # Loop over particles, build the results list, and check for errors.
        n = sampler.size()
        result = dict(step=i, particles=[])
        for j in range(n):
            # Errors?
            particle = sampler.particle(j)
            error = particle.get_exception()
            if error != 0:
                raise DNest4Error(error)

            # Results.
            result["particles"].append(particle.get_npy_coords())
        result["particles"] = np.array(result["particles"])

        # Loop over levels and save them.
        levels = sampler.get_levels()
        n = levels.size()
        result["levels"] = np.empty(n, dtype=[
            ("log_X", np.float64), ("log_likelihood", np.float64),
            ("tiebreaker", np.float64), ("accepts", np.uint16),
            ("tries", np.uint16), ("exceeds", np.uint16),
            ("visits", np.uint16)
        ])
        for j in range(n):
            level = levels[j]
            result["levels"][j]["log_X"] = level.get_log_X()
            result["levels"][j]["log_likelihood"] = \
                level.get_log_likelihood().get_value()
            result["levels"][j]["tiebreaker"] = \
                level.get_log_likelihood().get_tiebreaker()
            result["levels"][j]["accepts"] = level.get_accepts()
            result["levels"][j]["tries"] = level.get_tries()
            result["levels"][j]["exceeds"] = level.get_exceeds()
            result["levels"][j]["visits"] = level.get_visits()

        # Yield items as a generator.
        yield result

        # Hack to continue running.
        sampler.increase_max_num_saves(1)
        i += 1


class DNest4Error(Exception):
    pass
