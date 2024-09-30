#!/usr/bin/env python

import os
import sys
from setuptools import setup, Extension


if __name__ == "__main__":
    # The root of the DNest4 repo.
    basedir = os.path.dirname(os.path.abspath(__file__))

    # Publish the library to PyPI.
    if "publish" in sys.argv:
        os.system("python setup.py sdist upload")
        sys.exit()

    if "--without-cext" in sys.argv:
        sys.argv.remove("--without-cext")
        extensions = []

    else:
        import glob
        from itertools import chain

        import numpy
        from Cython.Build import cythonize

        # Set up the C++-extension.
        libraries = []
        extra_compile_args = [
            "-std=c++11",
            "-Wno-unused-function",
            "-Wno-uninitialized",
            "-DNO_THREADS",
            "-DNPY_NO_DEPRECATED_API"
        ]
        extra_link_args = ["-std=c++11"]
        if os.name == "posix":
            libraries.append("m")
        if sys.platform == "darwin":
            libraries.append("c++")
            extra_compile_args += ["-mmacosx-version-min=10.9"]
            extra_link_args += ["-mmacosx-version-min=10.9"]

        include_dirs = [
            "dnest4",
            os.path.join(basedir, "code"),
            numpy.get_include(),
        ]

        src = [
            os.path.join(basedir, "code", fn)
            for fn in chain(
                glob.glob(os.path.join(basedir, "code", "*.cpp")),
                glob.glob(
                    os.path.join(basedir, "code", "Distributions", "*.cpp")
                ),
                glob.glob(
                    os.path.join(
                        basedir,
                        "code",
                        "RJObject",
                        "ConditionalPriors",
                        "*.cpp",
                    )
                ),
            )
        ]
        src += [os.path.join(basedir, "python", "dnest4", "_dnest4.pyx")]

        ext = [
            Extension(
                "dnest4._dnest4",
                sources=src,
                language="c++",
                libraries=libraries,
                include_dirs=include_dirs,
                extra_compile_args=extra_compile_args,
                extra_link_args=extra_link_args,
            ),
            Extension(
                "dnest4.utils",
                sources=[
                    os.path.join(basedir, "python", "dnest4", "utils.pyx")
                ],
                language="c++",
                libraries=libraries,
                include_dirs=include_dirs,
                extra_compile_args=extra_compile_args,
                extra_link_args=extra_link_args,
            ),
        ]
        extensions = cythonize(ext)

    # Hackishly inject a constant into builtins to enable importing of the
    # package before the library is built.
    if sys.version_info[0] < 3:
        import __builtin__ as builtins
    else:
        import builtins
    builtins.__DNEST4_SETUP__ = True
    sys.path.insert(0, os.path.join(basedir, "python"))
    import dnest4

    setup(
        name="dnest4",
        version=dnest4.__version__,
        author="Daniel Foreman-Mackey",
        author_email="foreman.mackey@gmail.com",
        url="https://github.com/eggplantbren/DNest4",
        license="MIT",
        packages=["dnest4"],
        package_dir={"": "python"},
        ext_modules=extensions,
        description="Diffusive nested sampling in Python",
        long_description=open("python/README.rst").read(),
        classifiers=[
            # "Development Status :: 5 - Production/Stable",
            "Intended Audience :: Developers",
            "Intended Audience :: Science/Research",
            "License :: OSI Approved :: MIT License",
            "Operating System :: OS Independent",
            "Programming Language :: Python",
        ],
    )
