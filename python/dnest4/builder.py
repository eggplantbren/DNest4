from collections import OrderedDict
from enum import Enum

class Uniform:
    """
    Uniform distributions.
    """
    def __init__(self, a, b):
        self.a, self.b = a, b

    def from_prior(self):
        s = "{x} = {a} + ({b} - ({a}))*rng.rand();\n"
        return self.insert_parameters(s)

    def perturb(self):
        s  = "{x} += ({b} - ({a}))*rng.randh();\n"
        s += "wrap({x}, {a}, {b});\n"
        return self.insert_parameters(s)

    def log_density(self):
        s  = "if({x} < ({a}) || {x} > ({b}))\n"
        s += "    logp = -numeric_limits<double>::max();\n"
        s += "logp += -log({b} - ({a}));\n"
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
        s = "{x} = {mu} + {sigma}*rng.randn();\n"
        return self.insert_parameters(s)

    def perturb(self):
        s  = "log_H -= -0.5*pow(({x}) - ({mu}))/({sigma}), 2);\n"
        s += "{x} += ({sigma})*rng.randh();\n"
        s += "log_H += -0.5*pow((({x}) - ({mu}))/({sigma}), 2);\n"
        return self.insert_parameters(s)

    def log_density(self):
        s  = "logp += -log(2*M_PI) - log({sigma}) "
        s += "- 0.5*pow((({x}) - ({mu}))/({sigma}), 2);\n"
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
        self.index = index

    def from_prior(self):
        return self.prior.from_prior().replace("{x}", self.name)

    def perturb(self):
        return self.prior.perturb().replace("{x}", self.name)

    def log_density(self):
        return self.prior.log_density().replace("{x}", self.name)

    def __str__(self):
        if index is None:
            return self.name
        else:
            return self.name + "[" + str(index) + "]"


class Model:
    def __init__(self):
        self.nodes = OrderedDict()

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

        for name in self.nodes:
            node = self.nodes[name]
            if node.node_type == NodeType.derived:
                s += node.from_prior()
        return s

    def perturb(self):
        """
        Generate perturb code for the whole model.
        """
        # Count the number of coordinates
        num_coords = 0
        for name in self.nodes:
            node = self.nodes[name]
            if node.node_type == NodeType.coordinate:
                num_coords += 1

        # Choose which one to perturb
        s =  "double log_H = 0.0;\n"
        s += "int which = rng.rand_int({n});\n".replace("{n}", str(num_coords))

        #
        k = 0
        for name in self.nodes:
            node = self.nodes[name]
            if node.node_type == NodeType.coordinate:
                s += "if(which == {k})\n{".replace("{k}", str(k))
                s += node.perturb()
                s += "}"
                k += 1

        s += "return log_H;\n"
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

    def generate_h(self):
        """
        Load MyModel.h.template
        and fill in the required declarations.
        """
        # Prepare the declarations for MyModel.h
        declarations = ""

        # Declare non-vectors, accumulate names of vectors
        vectors = set()
        for name in self.nodes:
            node = self.nodes[name]
            if node.index is None:
                declarations += "        "
                declarations += "double {x};\n".replace("{x}", node.name)
            else:
                vectors.add(node.name)

        # Declare vectors
        declarations += "\n"
        for vec in vectors:
            declarations += "        "
            declarations += "std::vector<double> {x};".replace("{x}", vec)
            declarations += "\n"

        # Open the template .h file
        f = open("MyModel.h.template")
        s = f.read()
        s = s.replace("        {DECLARATIONS}", declarations)
        f.close()

        # Write the new .h file
        f = open("MyModel.h", "w")
        f.write(s)
        f.close()

        return s

if __name__ == "__main__":
    # Create a model
    model = Model()

    # Add three parameters to it
    model.add_node(Node("m", Uniform(-10.0, 10.0)))
    model.add_node(Node("b", Uniform(-10.0, 10.0)))
    model.add_node(Node("sigma", Uniform(0.0, 10.0)))

    # Add five data values
    for i in range(0, 5):
        model.add_node(Node("x", None, node_type=NodeType.prior_info, index=i))
        model.add_node(Node("y", Normal("m*x[i] + b", model.nodes["sigma"]),\
                                            node_type=NodeType.data, index=i))


    model.generate_h()


