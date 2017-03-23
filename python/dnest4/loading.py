# -*- coding: utf-8 -*-

import numpy as np

__all__ = ["my_loadtxt", "loadtxt_rows", "load_column_names"]

def my_loadtxt(filename, single_precision=False, delimiter=" "):
    """
    Load quickly
    """
    # Open the file
    f = open(filename, "r")

    # Storage
    results = []

    # Rows and columns
    nrow = 0
    ncol = None

    while(True):
        # Read the line and split by whitespace
        line = f.readline()
        if delimiter != " ":
            cells = line.split(delimiter)
        else:
            cells = line.split()

        # Quit when you see a different number of columns
        if ncol is not None and len(cells) != ncol:
            break

        # Non-comment lines
        if cells[0][0] != "#":
            # If it's the first one, get the number of columns
            if ncol is None:
                ncol = len(cells)

            # Otherwise, include in results
            if single_precision:
                results.append(np.array([float(cell) for cell in cells],\
                                                          dtype="float32"))
            else:
                results.append(np.array([float(cell) for cell in cells]))
            nrow += 1

    results = np.vstack(results)
    return results

def loadtxt_rows(filename, rows, single_precision=False):
    """
    Load only certain rows
    """
    # Open the file
    f = open(filename, "r")

    # Storage
    results = {}

    # Row number
    i = 0

    # Number of columns
    ncol = None

    while(True):
        # Read the line and split by whitespace
        line = f.readline()
        cells = line.split()

        # Quit when you see a different number of columns
        if ncol is not None and len(cells) != ncol:
            break

        # Non-comment lines
        if cells[0] != "#":
            # If it's the first one, get the number of columns
            if ncol is None:
                ncol = len(cells)

            # Otherwise, include in results
            if i in rows:
                if single_precision:
                    results[i] = np.array([float(cell) for cell in cells],\
                                                              dtype="float32")
                else:
                    results[i] = np.array([float(cell) for cell in cells])
            i += 1

    results["ncol"] = ncol
    return results

def load_column_names(filename):
    """
    Read the first line of the file and extract column names from it.
    Returns a dictionary with two elements:
        colnames        A list of column names
        indices         A dictionary of column indices, indexed by name
    """
    f = open(filename, "r")
    line = f.readline()
    f.close()

    names = line.replace("#", "").replace(" ", "").replace("\n", "")\
                .split(",")
    indices = {}
    for i in range(0, len(names)):
        indices[names[i]] = i

    return {"colnames": names, "indices": indices}

