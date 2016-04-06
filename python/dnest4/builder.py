from collections import OrderedDict
from enum import Enum
import numpy as np

__all__ = ["Uniform", "LogUniform", "Normal", "Cauchy", "Poisson",\
           "Exponential", "Binomial",\
           "Deterministic", "NodeType", "Node", "Model"]

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

    def log_prob(self):
        s  = "if({x} < ({a}) || {x} > ({b}))\n"
        s += "    logp = -numeric_limits<double>::max();\n"
        s += "logp += -log({b} - ({a}));\n"
        return self.insert_parameters(s)

    def insert_parameters(self, s):
        s = s.replace("{a}", str(self.a))
        s = s.replace("{b}", str(self.b))
        return s

class LogUniform:
    """
    Log-uniform distributions.
    """
    def __init__(self, a, b):
        self.a, self.b = a, b

    def from_prior(self):
        s = "{x} = exp(log({a}) + log(({b})/({a}))*rng.rand());\n"
        return self.insert_parameters(s)

    def perturb(self):
        s  = "{x} = log({x});\n"
        s += "{x} += log(({b})/({a}))*rng.randh();\n"
        s += "wrap({x}, log({a}), log({b}));\n"
        s += "{x} = exp({x});\n"
        return self.insert_parameters(s)

    def log_prob(self):
        s  = "if({x} < ({a}) || {x} > ({b}))\n"
        s += "    logp = -numeric_limits<double>::max();\n"
        s += "logp += -log({x}) - log(({b})/({a}));\n"
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
        s = "{x} = {mu} + ({sigma})*rng.randn();\n"
        return self.insert_parameters(s)

    def perturb(self):
        s  = "log_H -= -0.5*pow((({x}) - ({mu}))/({sigma}), 2);\n"
        s += "{x} += ({sigma})*rng.randh();\n"
        s += "log_H += -0.5*pow((({x}) - ({mu}))/({sigma}), 2);\n"
        return self.insert_parameters(s)

    def log_prob(self):
        s  = "logp += -0.5*log(2*M_PI) - log({sigma}) "
        s += "- 0.5*pow((({x}) - ({mu}))/({sigma}), 2);\n"
        return self.insert_parameters(s)

    def insert_parameters(self, s):
        s = s.replace("{mu}", str(self.mu))
        s = s.replace("{sigma}", str(self.sigma))
        return s


class Cauchy:
    """
    Cauchy distributions.
    """
    def __init__(self, mu, sigma):
        self.mu, self.sigma = mu, sigma

    def from_prior(self):
        s = "{x} = {mu} + ({sigma})*tan(M_PI*(rng.rand() - 0.5));\n"
        return self.insert_parameters(s)

    def perturb(self):
        s  = "{x} = 0.5 + atan(({x} - ({mu}))/({sigma}))/M_PI;\n"
        s += "{x} += rng.randh();\n"
        s += "wrap({x}, 0.0, 1.0);\n"
        s += "{x} = {mu} + ({sigma})*tan(M_PI*({x} - 0.5));\n"
        return self.insert_parameters(s)

    def log_prob(self):
        s  = "logp += -log(M_PI) - log({sigma}) "
        s += "- log(1.0 + pow(({x} - ({mu}))/({sigma}), 2));\n"
        return self.insert_parameters(s)

    def insert_parameters(self, s):
        s = s.replace("{mu}", str(self.mu))
        s = s.replace("{sigma}", str(self.sigma))
        return s

class Exponential:
    """
    Exponential distributions.
    """
    def __init__(self, mu):
        self.mu = mu

    def from_prior(self):
        s = "{x} = -({mu})*log(1.0 - rng.rand());\n"
        return self.insert_parameters(s)

    def perturb(self):
        s  = "{x} = 1.0 - exp(-{x}/({mu}));\n"
        s += "{x} += rng.randh();\n"
        s += "wrap({x}, 0.0, 1.0);\n"
        s += "{x} = -({mu})*log(1.0 - {x}));\n"
        return self.insert_parameters(s)

    def log_prob(self):
        s  = "if({x} < 0.0)\n"
        s += "    logp = -numeric_limits<double>::max();\n"
        s += "logp += -log({mu}) - {x}/({mu});\n"
        return self.insert_parameters(s)

    def insert_parameters(self, s):
        s = s.replace("{mu}", str(self.mu))
        return s

class Poisson:
    """
    Poisson distributions.
    SO FAR ONLY THE log_prob HAS BEEN IMPLEMENTED.
    """
    def __init__(self, mu):
        self.mu = mu

    def log_prob(self):
        s  = "if({x} < 0)\n"
        s += "    logp = -numeric_limits<double>::max();\n"
        s += "else\n"
        s += "    logp += {x}*log({mu}) - ({mu}) - lgamma({x} + 1);\n"
        return s

    def insert_parameters(self, s):
        s = s.replace("{mu}", str(self.mu))
        return s


class Binomial:
    """
    Binomial distributions.
    So far only the log_prob has been implemented.
    """
    def __init__(self, N, theta):
        self.N
        self.theta = theta

    def from_prior(self):
        pass

    def perturb(self):
        pass

    def log_prob(self):
        s  = "if({x} < 0 || {x} > ({N}))\n"
        s += "    logp = -numeric_limits<double>::max();\n"
        s += "else\n"
        s += "    lgamma({N} + 1) - lgamma({x} + 1) - lgamma({N} - ({x}) + 1)"
        s += "- lgamma({x} + 1);\n"
        return s

    def insert_parameters(self, s):
        s = s.replace("{N}", str(self.N))
        s = s.replace("{theta}", str(self.theta))
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
    def __init__(self, dtype, name, prior, node_type, index=None):
        self.dtype = dtype
        self.name  = name
        if index is not None:
            self.name += "[" + str(index) + "]"
        self.index = index
        self.prior = prior
        self.node_type = node_type
        if node_type == NodeType.prior_info:
            assert self.prior == None

    def from_prior(self):
        return self.prior.from_prior().replace("{x}", self.name)

    def perturb(self):
        return self.prior.perturb().replace("{x}", self.name)

    def log_prob(self):
        return self.prior.log_prob().replace("{x}", self.name)

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
                s += node.log_prob()
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

    def get_scalar_names(self, node_type):
        vecs = self.get_vector_names(node_type)
        scalars = set()
        for name in self.nodes:
            node = self.nodes[name]
            if node.node_type == node_type and\
               node.name.split("[")[0] not in vecs:
                scalars.add(node.name)
        return scalars

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
        # Vector knowns
        vecs = self.get_vector_names(NodeType.data)
        vecs = vecs.union(self.get_vector_names(NodeType.prior_info))
        declarations = "        // Data and prior information\n"
        for vec in vecs:
            declarations += "        static const std::vector"
            if self.nodes[vec + "[0]"].dtype == int:
                declarations += "<int>"
            else:
                declarations += "<double>"
            declarations += " {x};\n".format(x=vec)

        # Scalar knowns
        scalars = self.get_scalar_names(NodeType.data)
        scalars = scalars.union(self.get_scalar_names(NodeType.prior_info))
        for scalar in scalars:
            declarations += "        static const "
            if self.nodes[scalar].dtype == int:
                declarations += "int"
            else:
                declarations += "double"
            declarations += " {x};\n".format(x=scalar)

        declarations += \
                "\n        // Unknown parameters and derived quantities\n"

        # Vector unknowns
        vecs = self.get_vector_names(NodeType.coordinate)
        vecs = vecs.union(self.get_vector_names(NodeType.derived))
        for vec in vecs:
            declarations += "        std::vector"
            if self.nodes[vec + "[0]"].dtype == int:
                declarations += "<int>"
            else:
                declarations += "<double>"
            declarations += " {x};\n".format(x=vec)

        # Scalar unknowns
        scalars = self.get_scalar_names(NodeType.coordinate)
        scalars = scalars.union(self.get_scalar_names(NodeType.derived))
        for scalar in scalars:
            declarations += "        "
            if self.nodes[scalar].dtype == int:
                declarations += "int"
            else:
                declarations += "double"
            declarations += " {x};\n".format(x=scalar)

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
            initialiser_list += str(self.get_vector_size(vec)) + "), "
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

