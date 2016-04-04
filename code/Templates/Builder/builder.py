"""
Script to generate the StraightLine model in Python
"""
import numpy as np
from dnest4.builder import *

# The data and prior information in a dictionary
data = {}
data["x"] = np.array([1.0,   2.0,  3.0,  4.0,  5.0])
data["y"] = np.array([1.01, 1.99, 3.02, 3.88, 5.01])
data["N"] = 5

# Create a model
model = Model()

# Three unknown parameters
model.add_node(Node("m", Normal(0.0, 1000.0)))
model.add_node(Node("b", Normal(0.0, 1000.0)))
model.add_node(Node("sigma", LogUniform(1E-3, 1E3)))

# The data (need nodes for x, y, and N, to match the dictionary)
for i in range(0, data["N"]):
    model.add_node(Node("x", node_type=NodeType.prior_info, index=i))
    model.add_node(Node("y", Normal("m*x[{i}] + b".format(i=i),\
                                model.nodes["sigma"]),\
                                node_type=NodeType.data, index=i))
model.add_node(Node("N", node_type=NodeType.prior_info))

# Generate the .h file
model.generate_h()

# Generate the .cpp file
model.generate_cpp(data)

