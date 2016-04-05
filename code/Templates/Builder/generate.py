from builder import *

# Create a model
model = Model()

# Prior information nodes
model.add_node(Node(int, "N", None, NodeType.prior_info))
for i in range(0, 5):
    model.add_node(Node(float, "x", None, NodeType.prior_info, index=i))

# Data nodes (with sampling distribution!)
for i in range(0, 5):
    model.add_node(Node(float, "y",
                        Normal("m*x[{index}] + b".format(index=i), "sigma"),
                        NodeType.data, index=i))

# Unknown parameters
model.add_node(Node(float, "m", Normal(0.0, 1000.0), NodeType.coordinate))
model.add_node(Node(float, "b", Normal(0.0, 1000.0), NodeType.coordinate))
model.add_node(Node(float, "sigma", LogUniform(1E-3, 1E3), NodeType.coordinate))

model.generate_h()

