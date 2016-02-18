# -*- coding: utf-8 -*-

from .backends import MemoryBackend
from ._dnest4 import sample as _sample

__all__ = ["DNest4Sampler"]


class DNest4Sampler(object):

    def __init__(self, model, backend=None):
        self._model = model
        if backend is None:
            backend = MemoryBackend()
        self.backend = backend

    def sample(self, max_num_levels, **kwargs):
        self.backend.reset()
        for sample in _sample(self._model, max_num_levels, **kwargs):
            self.backend.write_particles(
                sample["samples"], sample["sample_info"]
            )
            self.backend.write_levels(sample["levels"])
            yield sample

    def run(self, num_steps, max_num_levels, **kwargs):
        if num_steps <= 0:
            raise ValueError("Invalid number of steps")
        kwargs["num_steps"] = int(num_steps)
        for _ in self.sample(max_num_levels, **kwargs):
            pass
