#ifndef DNest4_PyModel
#define DNest4_PyModel

#include <cmath>
#include <ostream>
#include <valarray>

#include <Python.h>
#include <numpy/arrayobject.h>

#include "DNest4.h"

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
            set_exception(-1);
            return;
        }

        // Parse that return value as a numpy array.
        PyObject* rarray = PyArray_FROM_OTF(result, NPY_DOUBLE, NPY_IN_ARRAY);
        if (result == NULL || (int)PyArray_NDIM(rarray) != 1) {
            Py_DECREF(result);
            Py_XDECREF(rarray);
            set_exception(-2);
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
        PyObject* c = get_npy_coords();

        // Call the Python method and get the Python return value.
        PyObject* result = PyObject_CallMethod(py_self_, "perturb", "O", c);
        if (result == NULL || PyErr_Occurred() != NULL) {
            Py_DECREF(c);
            Py_XDECREF(result);
            set_exception(2);
            return 0.0;
        }

        double log_H = PyFloat_AsDouble(result);
        Py_DECREF(result);
        if (PyErr_Occurred() != NULL) {
            Py_DECREF(c);
            set_exception(3);
            return 0.0;
        }

        double* data = (double*)PyArray_DATA(c);
        for (int i = 0; i < size_; ++i) coords_[i] = data[i];
        Py_DECREF(c);

        return log_H;
    };

    // Likelihood function
    double log_likelihood () {
        if (size_ == 0) return 0.0;

        PyObject* c = get_npy_coords();

        // Call the Python method and get the Python return value.
        PyObject* result = PyObject_CallMethod(py_self_, "log_likelihood", "O", c);
        Py_DECREF(c);
        if (result == NULL) {
            Py_XDECREF(result);
            set_exception(11);
            return -INFINITY;
        }

        // Parse as double.
        double log_like = PyFloat_AsDouble(result);
        if (PyErr_Occurred() != NULL) {
            Py_DECREF(result);
            set_exception(12);
            return -INFINITY;
        }

        return log_like;
    };

    PyObject* get_npy_coords () {
        npy_intp shape[] = {size_};
        PyObject* c = PyArray_SimpleNew(1, shape, NPY_DOUBLE);
        if (c == NULL) set_exception(-100);
        double* data = (double*)PyArray_DATA(c);
        for (int i = 0; i < size_; ++i) data[i] = coords_[i];
        return c;
    };

    // Print to stream
    void print(std::ostream& out) const {
        for (int i = 0; i < size_; ++i) {
            out << coords_[i];
            if (i < size_ - 1) out << " ";
        }
    };

    // Return string with column information
    std::string description() const { return "PyModel"; };

    // Getters, etc.
    void set_py_self (PyObject* py_self) { py_self_ = py_self; };
    PyObject* get_py_self () const { return py_self_; };

    // Dealing with exceptions.
    int get_exception () const { return exception_; };
    void set_exception (int exception) {
        if (exception_ == 0 && PyErr_Occurred() != NULL) {
            std::cerr << "The following Python exception occurred:\n";
            PyErr_Print();
        }
        exception_ = exception;
        throw exception_;
    };

private:

    PyObject* py_self_;
    int exception_, size_;
    std::valarray<double> coords_;
};

#endif
