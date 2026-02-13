DNest4
======

[![MIT License](https://img.shields.io/badge/license-MIT-blue.svg)](https://github.com/eggplantbren/DNest4/blob/master/LICENSE)

DNest4 is a C++11 implementation of Diffusive Nested Sampling, a Markov Chain
Monte Carlo (MCMC) algorithm for Bayesian Inference and Statistical Mechanics.
You can use it in a few different ways:

* Implement your model in C++, compile it and have it run super fast.
* Implement trans-dimensional models with the *RJObject* template class.
* Implement your model by writing just two functions in Python, R, or Julia (this is new and undocumented - email me or take a look in the Templates directory)
* Write up your model in Python, using a BUGS-style approach
[see here to learn how](https://plausibilitytheory.wordpress.com/2016/08/11/a-jags-like-interface-to-dnest4/)
(more documentation [here](https://odysee.com/@BrendonBrewer:3/dfs:5)).

Papers
======

There is a [paper](https://www.jstatsoft.org/article/view/v086i07)
describing DNest4 installation and usage in the Journal of
Statistical software. You might also want to read the original
[paper](http://arxiv.org/abs/0912.2380) describing the
Diffusive Nested Sampling algorithm itself.
If you find this software useful in your
research, please cite one or both of these papers. Here are the citations:

Brewer, B., & Foreman-Mackey, D. (2018).
DNest4: Diffusive Nested Sampling in C++ and Python.<br>
Journal of Statistical Software, 86(7), 1 - 33. doi:http://dx.doi.org/10.18637/jss.v086.i07

Brewer, B. J., Pártay, L. B., & Csányi, G. (2011). Diffusive nested sampling.<br>
Statistics and Computing, 21(4), 649-656.

NOTE: The Python model building described in the paper is now removed, and
there is no longer a Python package. Just a couple of helper scripts for simplicity.

Dependencies
============

You will need a C++ compiler that
supports the C++11 standard, along with Python 3 and the
Python packages NumPy and matplotlib.

Compiling the C++
=================

You can compile the DNest4 library (`libdnest4`) using the Makefile in the
`code` directory using:

```bash
cd code
make
```

Along with building the library this will compile all the examples,
which you can run by executing `main` in the appropriate example directory.
Then, for postprocessing, run `showresults.py` in each example directory.
This makes use the Python scripts from the `python`
directory within DNest4. I suggest adding this directory to your `PYTHONPATH`.

## Note for Mac users:
On some Macs, `g++` is an alias for `clang`, which is a C compiler. If this
is the case for you, you'll need to edit the first line of the Makefile so
that it uses `clang++`, which is a C++ compiler.

Alternative build process with SCons
====================================

However, you can also compile *and* install the library using [SCons](http://scons.org/).
To do this you just need to run:

```bash
scons install
```

By default it will attempt to install the library in `/usr/local` (with the library files in
`/usr/local/lib` and the headers in `/usr/local/include/dnest4`), so the above command
must be run as a user with root access or using `sudo`. To install to a different location
you can instead run:

```bash
scons install --prefix <install_location>
```

where `<install_location>` is the base path for the install.

To install with [GDB](https://www.gnu.org/software/gdb/) enabled during the library's compilation you can add the `--debug-mode`
flag to the install command.

Currently, the Scons installation does not compile the examples or the Python library. Any additions
to this installation process are welcome.

(c) 2015--2018 Brendon J. Brewer and contributors.
LICENCE: MIT.
See the LICENSE file for details.

*This work was supported by a Marsden Fast Start grant from the
Royal Society of New Zealand.*


