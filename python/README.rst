These are experimental Python bindings to the DNest4 sampler allowing the user
to implement their models in Python. See the example(s) in ``examples`` for
some demos for how you might do this.

Building
--------

DNest4 relies on recent C++11 features so building with python can be fun. On
a Mac, you might need to do something like:

.. code-block:: bash

    MACOSX_DEPLOYMENT_TARGET=10.9 python setup.py install

If you only want the postprocessing scripts and don't want to worry about
building the bindings, you can run:

.. code-block:: bash

    python setup.py install --without-cext

and then use it as follows:

.. code-block::

    import dnest

    # NOTE: the path will default to the current working directory if you
    #       don't give another path.
    dnest.postprocess("/path/to/where/you/ran/your/model")


Author
------

These bindings were written by `Dan Foreman-Mackey <https://github.com/dfm>`_
and they are licensed as part of DNest4 with the MIT license.
