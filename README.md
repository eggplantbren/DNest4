DNest4
======

DNest4 is a C++11 implementation of Diffusive Nested Sampling, a Markov Chain
Monte Carlo (MCMC) algorithm for Bayesian Inference and Statistical Mechanics.

Papers
======

There is a manuscript describing DNest4 installation and usage
in the paper/ directory of this repository.
You can compile it with pdflatex.

You might also want to read the paper describing the
Diffusive Nested Sampling algorithm. If you find this software useful in your
research, please cite this paper.

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
    Python packages NumPy, scipy, matplotlib, pandas, and Cython)
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

(c) 2015--2016 Brendon J. Brewer
LICENCE: MIT.
See the LICENSE file for details.

*This work was supported by a Marsden Fast Start grant from the
Royal Society of New Zealand.*


