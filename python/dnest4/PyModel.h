#ifndef DNest4_PyModel
#define DNest4_PyModel

#include <cmath>
#include <vector>
#include <ostream>

#include <Python.h>
#include <numpy/arrayobject.h>

#include "DNest4.h"

using std::vector;

class PyModel {
public:
    PyModel ()
        :py_self_(NULL)
        ,exception_(0)
        ,size_(0)
        ,coords_(0)
    {};

    void from_prior (DNest4::RNG& rng) {
        // Call the Python method and get the Python return value.
        PyObject* result = PyObject_CallMethod(py_self_, "from_prior", "");
        if (result == NULL) {
            Py_XDECREF(result);
            exception_ = -1;
            std::cerr << "DNest4 error: Python exception while calling 'from_prior'\n";
            PyErr_Print();
            return;
        }

        // Parse that return value as a numpy array.
        PyObject* rarray = PyArray_FROM_OTF(result, NPY_DOUBLE, NPY_IN_ARRAY);
        if (result == NULL || (int)PyArray_NDIM(rarray) != 1) {
            Py_DECREF(result);
            Py_XDECREF(rarray);
            exception_ = -2;
            std::cerr << "DNest4 error: 'from_prior' return value is invalid\n";
            PyErr_Print();
            return;
        }

        // Save the output as a C++ vector.
        size_ = (int)PyArray_DIM(rarray, 0);
        coords_.resize(size_);
        double* data = (double*)PyArray_DATA(rarray);
        for (int i = 0; i < size_; ++i) coords_[i] = data[i];

        // Clean up.
        Py_DECREF(result);
        Py_DECREF(rarray);
    };

    double perturb (DNest4::RNG& rng) {
        npy_intp shape[] = {size_};
        PyObject* c = PyArray_SimpleNewFromData(1, shape, NPY_DOUBLE, &(coords_[0]));
        if (c == NULL) {
            Py_XDECREF(c);
            exception_ = 1;
            std::cerr << "DNest4 error: failed to build coords vector\n";
            PyErr_Print();
            return -INFINITY;
        }

        // Call the Python method and get the Python return value.
        PyObject* result = PyObject_CallMethod(py_self_, "perturb", "O", c);
        Py_DECREF(c);
        double log_H = PyFloat_AsDouble(result);
        if (result == NULL || PyErr_Occurred() != NULL) {
            Py_XDECREF(result);
            exception_ = 2;
            std::cerr << "DNest4 error: Python exception while calling 'perturb'\n";
            PyErr_Print();
            return -INFINITY;
        }

        return log_H;
    };

    // Likelihood function
    double log_likelihood () {
        if (size_ == 0) return 0.0;

        npy_intp shape[] = {size_};
        PyObject* c = PyArray_SimpleNewFromData(1, shape, NPY_DOUBLE, &(coords_[0]));
        if (c == NULL) {
            Py_XDECREF(c);
            exception_ = 1;
            std::cerr << "DNest4 error: failed to build coords vector\n";
            PyErr_Print();
            return -INFINITY;
        }

        // Call the Python method and get the Python return value.
        PyObject* result = PyObject_CallMethod(py_self_, "log_likelihood", "O", c);
        Py_DECREF(c);
        double log_like = PyFloat_AsDouble(result);
        if (result == NULL || PyErr_Occurred() != NULL) {
            Py_XDECREF(result);
            exception_ = 2;
            std::cerr << "DNest4 error: Python exception while calling 'log_likelihood'\n";
            PyErr_Print();
            return -INFINITY;
        }

        return log_like;
    };

    PyObject* get_npy_coords () {
        npy_intp shape[] = {size_};
        return PyArray_SimpleNewFromData(1, shape, NPY_DOUBLE, &(coords_[0]));
    };

    // Print to stream
    void print(std::ostream& out) const {
        for (int i = 0; i < size_; ++i) {
            out << coords_[i];
            if (i < size_ - 1) out << " ";
        }
    };

    // Return string with column information
    std::string description() const {
        return "PyModel";
    };

    void set_py_self (PyObject* py_self) { py_self_ = py_self; };
    PyObject* get_py_self () const { return py_self_; };

    void set_exception (int exception) { exception_ = exception; };
    int get_exception () const { return exception_; };

private:

    PyObject* py_self_;
    int exception_, size_;
    vector<double> coords_;
};

#endif
