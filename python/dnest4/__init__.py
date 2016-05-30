# -*- coding: utf-8 -*-

__version__ = "0.0.1.dev0"

try:
    __DNEST4_SETUP__
except NameError:
    __DNEST4_SETUP__ = False

if not __DNEST4_SETUP__:
    __all__ = ["DNest4Sampler", "postprocess", "analysis"]

    from . import analysis
    from .sampler import DNest4Sampler
    from .deprecated import postprocess
    from .randh import randh

