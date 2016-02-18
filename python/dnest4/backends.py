# -*- coding: utf-8 -*-

import os
import numpy as np

__all__ = ["MemoryBackend", "CSVBackend"]


class MemoryBackend(object):

    def __init__(self):
        self.reset()

    def reset(self):
        self.samples = None
        self.sample_info = None
        self.levels = None

    def write_particles(self, samples, sample_info):
        if self._samples is None:
            self.samples = np.array([samples])
            self.sample_info = np.array([sample_info])
            return
        self.samples = np.append(self._samples, samples)
        self.sample_info = np.append(self._sample_info, sample_info)

    def write_levels(self, levels):
        self.levels = levels


class CSVBackend(object):

    def __init__(self, basedir, sep=None, fmt="%.18e"):
        self._sep = sep
        self._fmt = fmt

        self._levels_filename = os.path.join(basedir, "levels.txt")
        self._samples_filename = os.path.join(basedir, "sample.txt")
        self._sample_info_filename = os.path.join(basedir, "sample_info.txt")

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
        with open(self._samples_filename, "a") as f:
            info = sample_info[which]
            f.write("{0}\n".format(self.sep.join(
                "{0}".format(info["level_assignment"]),
                self._fmt % info["log_likelihood"],
                self._fmt % info["tiebreaker"],
                "{0}".format(which),
            )))

    def write_levels(self, levels):
        with open(self._levels_filename, "w") as f:
            f.write("# {0}\n".format(
                self.sep.join(
                    ("log_X", "log_likelihood", "tiebreaker", "accepts",
                     "tries", "exceeds", "visits")
                )
            ))

            for level in levels:
                f.write("{0}\n".format(self.sep.join(
                    self._fmt % level["log_X"],
                    self._fmt % level["log_likelihood"],
                    self._fmt % level["tiebreaker"],
                    "{0}".format(level["accepts"]),
                    "{0}".format(level["tries"]),
                    "{0}".format(level["exceeds"]),
                    "{0}".format(level["visits"]),
                )))

    @property
    def samples(self):
        sep = self.sep
        if not len(self.sep.strip()):
            sep = None
        return np.loadtxt(self._samples_filename, delimiter=sep)

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
