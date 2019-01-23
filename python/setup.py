#!/usr/bin/env python

import os

try:
    from setuptools import setup, Extension
except ImportError:
    from distutils.core import setup, Extension


if __name__ == "__main__":
    import sys

    # Publish the library to PyPI.
    if "publish" in sys.argv:
        os.system("python setup.py sdist upload")
        sys.exit()

    if "--without-cext" in sys.argv:
        sys.argv.remove("--without-cext")
        extensions = []

    else:
        import numpy
        from Cython.Build import cythonize

        # The root of the DNest4 repo.
        basedir = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

        # Set up the C++-extension.
        libraries = []
        if os.name == "posix":
            libraries.append("m")
        include_dirs = [
            "dnest4",
            os.path.join(basedir, "code"),
            numpy.get_include(),
        ]

        src = [os.path.join(basedir, "code", fn) for fn in [
            "Barrier.cpp",
            "CommandLineOptions.cpp",
            "Level.cpp",
            "LikelihoodType.cpp",
            "Options.cpp",
            "RNG.cpp",
            "Utils.cpp",
            "Distributions/Cauchy.cpp",
            "Distributions/ContinuousDistribution.cpp",
            "Distributions/Exponential.cpp",
            "Distributions/Gaussian.cpp",
            "Distributions/Jeffreys.cpp",
            "Distributions/Kumaraswamy.cpp",
            "Distributions/Laplace.cpp",
            "Distributions/LogUniform.cpp",
            "Distributions/Pareto.cpp",
            "Distributions/Rayleigh.cpp",
            "Distributions/Triangular.cpp",
            "Distributions/Uniform.cpp",
            "RJObject/ConditionalPriors/BasicCircular.cpp",
            "RJObject/ConditionalPriors/ClassicMassInf1D.cpp",
            "RJObject/ConditionalPriors/ClassicMassInf.cpp",
            "RJObject/ConditionalPriors/ConditionalPrior.cpp"
        ]]
        src += [
            os.path.join("dnest4", "_dnest4.pyx")
        ]

        ext = [Extension(
            "dnest4._dnest4",
            sources=src,
            language="c++",
            libraries=libraries,
            include_dirs=include_dirs,
            extra_compile_args=["-std=c++11",
                                "-Wno-unused-function",
                                "-Wno-uninitialized",
                                "-DNO_THREADS"],
            extra_link_args=["-std=c++11"],
        ),
        Extension(
            "dnest4.utils",
            sources=[os.path.join("dnest4", "utils.pyx")],
            language="c++",
            libraries=libraries,
            include_dirs=include_dirs,
            extra_compile_args=["-std=c++11",
                                "-Wno-unused-function",
                                "-Wno-uninitialized",
                                "-DNO_THREADS"],
            extra_link_args=["-std=c++11"],
            cython_directives={"cdivision": True}
        )]
        extensions = cythonize(ext)

    # Hackishly inject a constant into builtins to enable importing of the
    # package before the library is built.
    if sys.version_info[0] < 3:
        import __builtin__ as builtins
    else:
        import builtins
    builtins.__DNEST4_SETUP__ = True
    import dnest4

    setup(
        name="dnest4",
        version=dnest4.__version__,
        author="Daniel Foreman-Mackey",
        author_email="foreman.mackey@gmail.com",
        url="https://github.com/eggplantbren/DNest4",
        license="MIT",
        packages=["dnest4"],
        ext_modules=extensions,
        description="Diffusive nested sampling in Python",
        long_description=open("README.rst").read(),
        classifiers=[
            # "Development Status :: 5 - Production/Stable",
            "Intended Audience :: Developers",
            "Intended Audience :: Science/Research",
            "License :: OSI Approved :: MIT License",
            "Operating System :: OS Independent",
            "Programming Language :: Python",
        ],
    )
