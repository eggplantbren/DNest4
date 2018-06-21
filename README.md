DNest4
======

[![Build Status](https://travis-ci.org/eggplantbren/TwinPeaks2018.svg?branch=master)](https://travis-ci.org/eggplantbren/TwinPeaks2018)
[![MIT License](https://img.shields.io/badge/license-MIT-blue.svg)](https://github.com/eggplantbren/DNest4/blob/master/LICENSE)

DNest4 is a C++11 implementation of Diffusive Nested Sampling, a Markov Chain
Monte Carlo (MCMC) algorithm for Bayesian Inference and Statistical Mechanics.

Papers
======

There is a manuscript describing DNest4 installation and usage
in the paper/ directory of this repository.
You can compile it with pdflatex. Alternatively, you can get the
preprint [here](http://arxiv.org/abs/1606.03757).

You might also want to read the paper describing the
Diffusive Nested Sampling algorithm. If you find this software useful in your
research, please cite one or both of these papers.

Diffusive Nested Sampling
Brendon J. Brewer, Livia B. Pártay, and Gábor Csányi
Statistics and Computing, 2011, 21, 4, 649-656.

The algorithm
paper is [freely available online](http://arxiv.org/abs/0912.2380) at
the [arXiv](http://www.arxiv.org/).

Improvements over DNest3:
=========================

*	There are far fewer dependencies --- all you need is a C++ compiler that
	supports the C++11 standard, along with Python (and the
    Python packages NumPy, scipy, matplotlib, and Cython)
	for the post-processing scripts. Because of this, it should be much easier
	to compile (at least on a Unix-like operating system such as Ubuntu or
	Mac OS X).

*	The licence is now the permissive MIT licence.

*	RJObject (which allows relatively straightforward implementation of
	hierarchical and trans-dimensional mixture models)
	is now included in the same repository.

*   There is now some support for implementing models in Python and Julia,
    making DNest4 usable to non-C++ programmers.

Some functions have slightly different names and specifications compared to
DNest3. These changes are cosmetic, but mean that porting models from DNest3
to DNest4 will take a little bit of work.

Compiling
=========

## Note for Mac users:
On some Macs, `g++` is an alias for `clang`, which is a C compiler. If this
is the case for you, you'll need to edit the first line of the Makefile so
that it uses `clang++`, which is a C++ compiler.

You can compile the DNest4 library (`libdnest4`) using the Makefile in the `code` directory
using:

```bash
cd code
make
```

Along with building the library this will compile all the examples.
Then, install the Python package:

```
cd ../python
python setup.py install
```

In Mac OS X, the final line (which installs the Python parts) of DNest4) needs to provide
information about your OS version. For example, if your computer runs Mac OS X 10.9, the
installation command for the Python package is

```
MACOSX_DEPLOYMENT_TARGET=10.9 python setup.py install
```



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


