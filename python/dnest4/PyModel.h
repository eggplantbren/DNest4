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
        ,npy_coords_(NULL)
        ,exception_(0)
        ,size_(0)
        ,coords_(0)
    {};

    //
    // FIXME: there's a small chance that there's a memory leak here but I
    //        can't quite work it out --- DECREF-ing here segfaults.
    //
    // ~PyModel () {
    //     if (npy_coords_ != NULL) Py_DECREF(npy_coords_);
    // }

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

        // Save this as a numpy array too.
        npy_intp shape[] = {size_};
        npy_coords_ = PyArray_SimpleNewFromData(1, shape, NPY_DOUBLE, &(coords_[0]));
        if (npy_coords_ == NULL) set_exception(-3);
    };

    double perturb (DNest4::RNG& rng) {
        // Call the Python method and get the Python return value.
        PyObject* result = PyObject_CallMethod(py_self_, "perturb", "O", npy_coords_);
        if (result == NULL || PyErr_Occurred() != NULL) {
            Py_XDECREF(result);
            set_exception(2);
            return 0.0;
        }

        double log_H = PyFloat_AsDouble(result);
        Py_DECREF(result);
        if (PyErr_Occurred() != NULL) {
            set_exception(3);
            return 0.0;
        }

        return log_H;
    };

    // Likelihood function
    double log_likelihood () {
        if (size_ == 0) return 0.0;

        // Call the Python method and get the Python return value.
        PyObject* result = PyObject_CallMethod(py_self_, "log_likelihood", "O", npy_coords_);
        double log_like = PyFloat_AsDouble(result);
        if (result == NULL || PyErr_Occurred() != NULL) {
            Py_XDECREF(result);
            set_exception(11);
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
    PyObject* npy_coords_;
    int exception_, size_;
    vector<double> coords_;
};

#endif
