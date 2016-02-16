#!/usr/bin/env python
# -*- coding: utf-8 -*-

import numpy as np

from dnest4.analysis import interpolate_samples

np.random.seed(123)

nsteps = 50
ndim = 2

samples = np.random.randn(nsteps, ndim)
sample_info = np.array(list(zip(
    np.arange(nsteps),
    np.append(np.random.uniform(-2e4, -100.0, size=nsteps-2), [-1e4, -1e4]),
    np.random.rand(nsteps),
)), dtype=[
    ("level_assignment", np.uint16),
    ("log_likelihood", np.float64),
    ("tiebreaker", np.float64),
])

nlevels = 4
levels = np.array(list(zip(
    [0.0, -0.97, -1.5, -2.11],
    [-np.inf, -1e4, -1e4, -3012.4],
    [0.1, 0.2, 0.9, 0.1],
    np.zeros(nlevels, dtype=int),
    np.zeros(nlevels, dtype=int),
    np.zeros(nlevels, dtype=int),
    np.zeros(nlevels, dtype=int),
)), dtype=[
    ("log_X", np.float64), ("log_likelihood", np.float64),
    ("tiebreaker", np.float64), ("accepts", np.uint16),
    ("tries", np.uint16), ("exceeds", np.uint16),
    ("visits", np.uint16)
])


interpolate_samples(levels, sample_info, resample=True)
