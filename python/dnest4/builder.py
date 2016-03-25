from enum import Enum

class Uniform:
    """
    Uniform distributions.
    """
    def __init__(self, a, b):
        self.a, self.b = a, b

    def from_prior(self):
        s = r"{x} = {a} + ({b} - ({a}))*rng.rand();\n"
        return self.insert_parameters(s)

    def perturb(self):
        s  = r"{x} += ({b} - ({a}))*rng.randh();\n"
        s += r"wrap({x}, {a}, {b});\n"
        return self.insert_parameters(s)

    def log_density(self):
        s  = r"if({x} < ({a}) || {x} > ({b}))\n"
        s += r"    logp = -numeric_limits<double>::max();\n"
        s += r"logp += -log({b} - ({a}));\n"
        return self.insert_parameters(s)

    def insert_parameters(self, s):
        s = s.replace("{a}", str(self.a))
        s = s.replace("{b}", str(self.b))
        return s

class Normal:
    """
    Normal distributions.
    """
    def __init__(self, mu, sigma):
        self.mu, self.sigma = mu, sigma

    def from_prior(self):
        s = r"{x} = {mu} + {sigma}*rng.randn();\n"
        return self.insert_parameters(s)

    def perturb(self):
        s  = r"log_H -= -0.5*pow(({x}) - ({mu}))/({sigma}), 2);\n"
        s += r"{x} += ({sigma})*rng.randh();\n"
        s += r"log_H += -0.5*pow((({x}) - ({mu}))/({sigma}), 2);\n"
        return self.insert_parameters(s)

    def log_density(self):
        s  = r"logp += -log(2*M_PI) - log({sigma}) "
        s += r"- 0.5*pow((({x}) - ({mu}))/({sigma}), 2);\n"
        return self.insert_parameters(s)

    def insert_parameters(self, s):
        s = s.replace("{mu}", str(self.mu))
        s = s.replace("{sigma}", str(self.sigma))
        return s

class NodeType(Enum):
    """
    To distinguish between different kinds of Nodes
    """
    coordinate = 1
    derived = 2
    data = 3
    prior_info = 4

class Node:
    """
    A single parameter or data value.
    """
    def __init__(self, name, prior, node_type=NodeType.coordinate, index=None):
        self.name = name
        self.prior = prior
        self.node_type = node_type
        if index is not None:
            self.name += "[" + str(index) + "]"

    def from_prior(self):
        return self.prior.from_prior().replace("{x}", self.name)

    def perturb(self):
        return self.prior.perturb().replace("{x}", self.name)

    def log_density(self):
        return self.prior.log_density().replace("{x}", self.name)

    def __str__(self):
        return self.name


class Model:
    def __init__(self):
        self.nodes = {}

    def add_node(self, node):
        self.nodes[node.name] = node

    def from_prior(self):
        """
        Generate the from_prior code for the whole model.
        """
        s = ""
        for name in self.nodes:
            node = self.nodes[name]
            if node.node_type == NodeType.coordinate:
                s += node.from_prior()
        return s

    def log_likelihood(self):
        """
        Generate the log_likelihood code for the whole model.
        """
        s = ""
        for name in self.nodes:
            node = self.nodes[name]
            if node.node_type == NodeType.data:
                s += node.log_density()
        return s


if __name__ == "__main__":
    # Create a model
    model = Model()

    # Add two parameters to it
    model.add_node(Node("m", Uniform(-10.0, 10.0)))
    model.add_node(Node("b", Uniform(-10.0, 10.0)))
    model.add_node(Node("sigma", Uniform(0.0, 10.0)))

    # Add five data values
    for i in range(0, 5):
        model.add_node(Node("x", None, node_type=NodeType.prior_info, index=i))
        model.add_node(Node("y", Normal("m*x[i] + b", model.nodes["sigma"]),\
                                        node_type=NodeType.data, index=i))

    # Print out from_prior code
    print(model.from_prior())

    # Print out log_likelihood code
    print(model.log_likelihood())

