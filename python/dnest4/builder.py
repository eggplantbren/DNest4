from collections import OrderedDict
from enum import Enum
import numpy as np

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
        s  = "logp += -0.5*log(2*M_PI) - log({sigma}) "
        s += "- 0.5*pow((({x}) - ({mu}))/({sigma}), 2);\n"
        return self.insert_parameters(s)

    def insert_parameters(self, s):
        s = s.replace("{mu}", str(self.mu))
        s = s.replace("{sigma}", str(self.sigma))
        return s

class Deterministic:
    """
    For deterministic nodes --- a delta-function distribution :)
    """
    def __init__(self, formula):
        self.formula = formula

    def from_prior(self):
        s = "{x} = {formula};\n"
        s = s.replace("{formula}", self.formula)
        return s

    def perturb(self):
        return self.from_prior()

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
    def __init__(self, name, prior=None, index=None,\
                    node_type=NodeType.coordinate, dtype=float):
        self.name = name
        self.prior = prior
        self.index = index
        self.node_type = node_type
        self.dtype = dtype
        
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

        # The if-statements
        k = 0
        for name in self.nodes:
            node = self.nodes[name]
            if node.node_type == NodeType.coordinate:
                s += "if(which == {k})\n{\n".replace("{k}", str(k))
                s += "\n".join(["    " + x for x in node.perturb().splitlines()])
                s += "\n}\n"
                k += 1

        # Recompute derived nodes
        for name in self.nodes:
            node = self.nodes[name]
            if node.node_type == NodeType.derived:
                s += node.from_prior()

        s += "return log_H;\n"
        return s

    def print_code(self):
        """
        Generate print code for the whole model.
        """
        s = ""
        for name in self.nodes:
            node = self.nodes[name]
            if node.node_type == NodeType.coordinate:
                s += "out<<" + str(node) + "<<\" \";\n"
        return s

    def description(self):
        """
        Generate description code for the whole model.
        """
        s = "string s;\n"
        for name in self.nodes:
            node = self.nodes[name]
            if node.node_type == NodeType.coordinate:
                s += "s += \"" + str(node) + ", \";\n"
        s = s[0:-5]
        s += "\";"
        s += "\nreturn s;"
        return s

    def log_likelihood(self):
        """
        Generate the log_likelihood code for the whole model.
        """
        s = "double logp = 0.0;\n\n"
        for name in self.nodes:
            node = self.nodes[name]
            if node.node_type == NodeType.data:
                s += node.log_density()
        s += "\nreturn logp;"
        return s

    def get_vector_names(self, node_type):
        """
        Return a set of names of vectors of a certain NodeType.
        """
        vecs = set()
        for name in self.nodes:
            node = self.nodes[name]
            if node.node_type == node_type and\
               node.index is not None:
                vecs.add(name.split("[")[0])
        return vecs

    def get_vector_size(self, vector_name):
        count = 0
        for name in self.nodes:
            if name.split("[")[0] == vector_name:
                count += 1
        return count

    def generate_h(self):
        """
        Load MyModel.h.template
        and fill in the required declarations.
        """
        # Prepare the declarations for MyModel.h
        declarations = ""

        # Declare scalar unknowns
        for name in self.nodes:
            node = self.nodes[name]
            if node.index is None and (node.node_type == NodeType.coordinate or\
                                       node.node_type == NodeType.derived):
                declarations += "        "
                declarations += "double {x};\n".replace("{x}", node.name)

        # Declare vector unknowns
        vecs = self.get_vector_names(NodeType.coordinate)
        vecs = vecs.union(self.get_vector_names(NodeType.derived))
        for vec in vecs:
            declarations += "        "
            declarations += "std::vector<double> {x};\n".replace("{x}",\
                             vec)

        declarations += "\n"

        # Declare scalar knowns
        for name in self.nodes:
            node = self.nodes[name]
            if node.index is None and (node.node_type == NodeType.data or\
                                       node.node_type == NodeType.prior_info):
                declarations += "        static const "
                if node.dtype == float:
                    declarations += "double "
                elif node.dtype == int:
                    declarations += "int "
                declarations += "{x};\n".replace("{x}", node.name)
        declarations += "\n"

        # Declare vector knowns
        vecs = self.get_vector_names(NodeType.data)
        vecs = vecs.union(self.get_vector_names(NodeType.prior_info))
        for vec in vecs:
            declarations += "        static const "
            declarations += "std::vector<double> {x};\n".replace("{x}",\
                             vec)

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


    def generate_cpp(self, data):
        """
        Load MyModel.cpp.template
        and fill in the required stuff.
        """
        # Prepare the from_prior code
        from_prior = self.from_prior()
        from_prior = ["    " + x for x in from_prior.splitlines()]
        from_prior = "\n".join(from_prior)

        # Prepare the perturb code
        perturb = self.perturb()
        perturb = ["    " + x for x in perturb.splitlines()]
        perturb = "\n".join(perturb)

        # Prepare the log_likelihood code
        log_likelihood = self.log_likelihood()
        log_likelihood = ["    " + x for x in log_likelihood.splitlines()]
        log_likelihood = "\n".join(log_likelihood)

        # Prepare the print code
        print_code = self.print_code()
        print_code = ["    " + x for x in print_code.splitlines()]
        print_code = "\n".join(print_code)

        # Prepare the description code
        description = self.description()
        description = ["    " + x for x in description.splitlines()]
        description = "\n".join(description)

        # Prepare the data
        the_data = ""
        for d in data:
            the_data += "const "
            if type(data[d]) == np.ndarray:
                if data[d].dtype == np.dtype('float64'):
                    the_data += "std::vector<double> MyModel::"
                elif data[d].dtype == np.dtype('int64'):
                    the_data += "std::vector<int> MyModel::"
                else:
                    print("Unsupported dtype.")
            if type(data[d]) == float:
                the_data += "double MyModel::"
            if type(data[d]) == int:
                the_data += "int MyModel::"

            the_data += str(d) + "{"
            if type(data[d]) == np.ndarray:
                for value in data[d]:
                    the_data += str(value) + ", "
            else:
                the_data += str(data[d]) + ", "
            the_data = the_data[0:-2]
            the_data += "};\n"

        # Prepare the initialiser list, which just needs
        # vector unknowns to have the right size.
        initialiser_list = ":"
        vecs = self.get_vector_names(NodeType.coordinate)
        vecs = vecs.union(self.get_vector_names(NodeType.derived))
        for vec in vecs:
            initialiser_list += vec + "("
            initialiser_list += str(self.get_vector_size(vec)) + ")\n,"
        initialiser_list = initialiser_list[0:-2]

        # Open the template .cpp file
        f = open("MyModel.cpp.template")
        s = f.read()

        # Do the replacements
        s = s.replace("{FROM_PRIOR}", from_prior)
        s = s.replace("{PERTURB}", perturb)
        s = s.replace("{LOG_LIKELIHOOD}", log_likelihood)
        s = s.replace("{PRINT}", print_code)
        s = s.replace("{DESCRIPTION}", description)
        s = s.replace("{STATIC_DECLARATIONS}", the_data)
        s = s.replace("{INITIALIZER_LIST}", initialiser_list)
        f.close()

        # Write the new .h file
        f = open("MyModel.cpp", "w")
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

    # A derived parameter
    model.add_node(Node("variance", Deterministic("pow(sigma, 2)"),\
                                    node_type=NodeType.derived))

    # A vector of parameters
    for i in range(0, 3):
        model.add_node(Node("theta", Uniform(0.0, 1.0), index=i))

    # A vector of parameters
    for i in range(0, 7):
        model.add_node(Node("phi", Normal(10.0, 1.0), index=i))

    # Add data and prior information values
    model.add_node(Node("N", Deterministic("5"),
                             node_type=NodeType.prior_info, dtype=int))
    for i in range(0, 5):
        model.add_node(Node("x", 3.2, node_type=NodeType.prior_info, index=i))
        model.add_node(Node("y", Normal("m*x[{i}] + b".format(i=i),\
                                    model.nodes["sigma"]),\
                                    node_type=NodeType.data, index=i))

    # The data and prior information in a dictionary
    data = {}
    data["x"] = np.array([1.0,   2.0,  3.0,  4.0,  5.0])
    data["y"] = np.array([1.01, 1.99, 3.02, 3.88, 5.01])
    data["N"] = 5

    # Generate the .h file
    model.generate_h()

    # Generate the .cpp file
    model.generate_cpp(data)

