import numpy as np
import dnest4.builder as bd

data = {"x": np.array([1.0, 2.0, 3.0, 4.0, 5.0]),\
        "y": np.array([1.0, 2.0, 3.0, 3.9, 5.1]),\
        "N": 5}

# Create the model
model = bd.Model()

# Slope and intercept
model.add_node(bd.Node("m", bd.Uniform(-100.0, 100.0)))
model.add_node(bd.Node("b", bd.Uniform(-100.0, 100.0)))

# Noise standard deviation
model.add_node(bd.Node("log_sigma", bd.Uniform(-10.0, 10.0)))
model.add_node(bd.Node("sigma", bd.Delta("exp(log_sigma)")))

# p(data | parameters)
for i in range(0, data["N"]):
    name = "y{index}".format(index=i)
    mean = "m*x{index} + b".format(index=i)
    model.add_node(bd.Node(name, bd.Normal(mean, "sigma"), observed=True))

# Create the C++ code
bd.generate_h(model, data)
bd.generate_cpp(model, data)

# Compile the C++ code so it's ready to go
import os
os.system("make")

