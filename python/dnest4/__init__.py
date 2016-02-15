# -*- coding: utf-8 -*-

__version__ = "0.0.1.dev0"

try:
    __DNEST4_SETUP__
except NameError:
    __DNEST4_SETUP__ = False

if not __DNEST4_SETUP__:
    __all__ = ["sample", "postprocess"]

    from ._dnest4 import sample
    from .analysis import postprocess
