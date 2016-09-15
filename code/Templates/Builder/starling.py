import numpy as np

# Import DNest4's builder
import dnest4.builder as bd

# The data, as a dictionary
data = {}
data["N"] = 40
data["N_groups"] = 4
data["group"] = np.array([0, 0, 0, 0, 0, 0, 0, 0, 0, 0,\
                          1, 1, 1, 1, 1, 1, 1, 1, 1, 1,\
                          2, 2, 2, 2, 2, 2, 2, 2, 2, 2,\
                          3, 3, 3, 3, 3, 3, 3, 3, 3, 3], dtype="int64")
data["y"] = np.array([78, 88, 87, 88, 83, 82, 81, 80, 80, 89, 78, 78, 83,\
                      81, 78, 81, 81, 82, 76, 76, 79, 73, 79, 75, 77, 78,\
                      80, 78, 83, 84, 77, 69, 75, 70, 74, 83, 80, 75, 76,\
                      75], dtype="float64")

# A model
model = bd.Model()

# Hyperparameters
model.add_node(bd.Node("grand_mean", bd.Normal(0.0, 1000.0)))
model.add_node(bd.Node("diversity", bd.LogUniform(1E-3, 1E3)))

# Group mean parameters
for i in range(0, data["N_groups"]):
    model.add_node(bd.Node("n{i}".format(i=i), bd.Normal(0.0, 1.0)))
    model.add_node(\
                    bd.Node("mu{i}".format(i=i),\
                         bd.Delta("grand_mean + diversity*n{i}".format(i=i))\
                        )\
                  )

# Noise sd
model.add_node(bd.Node("sigma", bd.LogUniform(1E-3, 1E3)))

# Data nodes
for i in range(0, data["N"]):
    name = "y{i}".format(i=i)
    model.add_node(bd.Node(name,\
                        bd.Normal("mu{group}".format(group=data["group"][i]),\
                                    "sigma"), observed=True))

# Create the C++ code
bd.generate_h(model, data)
bd.generate_cpp(model, data)

# Compile the C++ code so it's ready to go
import os
os.system("make")


