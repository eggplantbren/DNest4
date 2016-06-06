# -*- coding: utf-8 -*-

import os
import numpy as np
from .loading import my_loadtxt, loadtxt_rows

__all__ = ["MemoryBackend", "CSVBackend"]


class MemoryBackend(object):

    def __init__(self):
        self.reset()

    def reset(self):
        self.samples = None
        self.sample_info = None
        self.levels = None
        self.sample_log_X = None
        self.weights = None
        self.posterior_samples = None
        self.stats = None

    def write_particles(self, samples, sample_info):
        if self.samples is None:
            self.samples = np.array([samples])
            self.sample_info = np.array([sample_info])
            return
        self.samples = np.concatenate((self.samples, [samples]), axis=0)
        self.sample_info = np.concatenate((self.sample_info, [sample_info]),
                                          axis=0)

    def write_levels(self, levels):
        self.levels = levels

    def write_sample_log_X(self, sample_log_X):
        self.sample_log_X = sample_log_X.reshape(self.sample_info.shape)

    def write_weights(self, weights):
        self.weights = weights.reshape(self.sample_info.shape)

    def write_posterior_samples(self, new_samples):
        self.posterior_samples = new_samples

    def write_stats(self, stats):
        self.stats = stats


class CSVBackend(object):

    def __init__(self, basedir, sep=None, fmt="%.18e"):
        try:
            os.makedirs(basedir)
        except os.error:
            pass

        self._sep = sep
        self._fmt = fmt

        self._levels_filename = os.path.join(basedir, "levels.txt")
        self._samples_filename = os.path.join(basedir, "sample.txt")
        self._sample_info_filename = os.path.join(basedir, "sample_info.txt")
        self._sample_log_X_filename = os.path.join(basedir, "sample_log_X.txt")
        self._weights_filename = os.path.join(basedir, "weights.txt")
        self._posterior_samples_filename = os.path.join(
            basedir, "posterior_sample.txt"
        )
        self._stats_filename = os.path.join(basedir, "stats.txt")

    @property
    def sep(self):
        if self._sep is None:
            self._sep = ","
            if os.path.exists(self._levels_filename):
                with open(self._levels_filename, "r") as f:
                    for i, line in enumerate(f):
                        if i > 1:
                            if "," not in line:
                                self._sep = " "
                                break
        return self._sep

    def reset(self):
        sep = self.sep
        with open(self._levels_filename, "w") as f:
            f.write("# {0}\n".format(
                sep.join(("log_X", "log_likelihood", "tiebreaker", "accepts",
                          "tries", "exceeds", "visits"))
            ))
        with open(self._samples_filename, "w") as f:
            pass
        with open(self._sample_info_filename, "w") as f:
            f.write("# {0}\n".format(
                sep.join(("level_assignment", "log_likelihood", "tiebreaker",
                          "ID"))
            ))

    def write_particles(self, samples, sample_info):
        which = np.random.randint(len(samples))
        with open(self._samples_filename, "a") as f:
            f.write("{0}\n".format(
                self.sep.join((self._fmt % s for s in samples[which]))
            ))
        with open(self._sample_info_filename, "a") as f:
            info = sample_info[which]
            f.write("{0}\n".format(self.sep.join((
                "{0}".format(info["level_assignment"]),
                self._fmt % info["log_likelihood"],
                self._fmt % info["tiebreaker"],
                "{0}".format(which),
            ))))

    def write_levels(self, levels):
        with open(self._levels_filename, "w") as f:
            f.write("# {0}\n".format(
                self.sep.join(
                    ("log_X", "log_likelihood", "tiebreaker", "accepts",
                     "tries", "exceeds", "visits")
                )
            ))

            for level in levels:
                f.write("{0}\n".format(self.sep.join((
                    self._fmt % level["log_X"],
                    self._fmt % level["log_likelihood"],
                    self._fmt % level["tiebreaker"],
                    "{0}".format(level["accepts"]),
                    "{0}".format(level["tries"]),
                    "{0}".format(level["exceeds"]),
                    "{0}".format(level["visits"]),
                ))))

    def write_posterior_samples(self, samples):
        np.savetxt(self._posterior_samples_filename, samples, fmt=self._fmt,
                   delimiter=self.sep)

    def write_weights(self, weights):
        np.savetxt(self._weights_filename, weights, fmt=self._fmt,
                   delimiter=self.sep)

    def write_sample_log_X(self, sample_log_X):
        np.savetxt(self._sample_log_X_filename, sample_log_X, fmt=self._fmt,
                   delimiter=self.sep)

    def write_stats(self, stats):
        with open(self._stats_filename, "w") as f:
            f.write("\n".join((self.sep.join(map("{0}".format, _))
                               for _ in stats.items())))

    @property
    def samples(self):
        sep = self.sep
        if not len(self.sep.strip()):
            sep = None
        return np.atleast_2d(my_loadtxt(self._samples_filename, delimiter=sep))

    @property
    def levels(self):
        with open(self._levels_filename, "r") as f:
            return np.array(
                [tuple(line.split(self.sep)) for line in f
                 if not line.startswith("#")],
                dtype=[
                    ("log_X", float), ("log_likelihood", float),
                    ("tiebreaker", float), ("accepts", int),
                    ("tries", int), ("exceeds", int), ("visits", int)
                ]
            )

    @property
    def sample_info(self):
        with open(self._sample_info_filename, "r") as f:
            return np.array(
                [tuple(line.split(self.sep)) for line in f
                 if not line.startswith("#")],
                dtype=[
                    ("level_assignment", int), ("log_likelihood", float),
                    ("tiebreaker", float), ("ID", int),
                ]
            )

    @property
    def sample_log_X(self):
        sep = self.sep
        if not len(self.sep.strip()):
            sep = None
        return np.atleast_2d(my_loadtxt(self._sample_log_X_filename,\
                                            delimiter=sep))

    @property
    def weights(self):
        sep = self.sep
        if not len(self.sep.strip()):
            sep = None
        return my_loadtxt(self._weights_filename, delimiter=sep)

    @property
    def posterior_samples(self):
        sep = self.sep
        if not len(self.sep.strip()):
            sep = None
        return np.atleast_2d(my_loadtxt(self._posterior_samples_filename,\
                                                                 delimiter=sep))

    @property
    def stats(self):
        stats = dict()
        with open(self._stats_filename, "r") as f:
            for line in f:
                k, v = line.split(self.sep)
                stats[k] = float(v)
        return stats
