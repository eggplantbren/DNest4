from __future__ import print_function

import numpy as np
from .distributions import *

__all__ = ["Model", "Node", "data_declaration", "data_definition",\
            "generate_h", "generate_cpp"]

class Model:
    def __init__(self):
        self.nodes = []
        self.indices = {}
        self.num_params = 0

    def __getitem__(self, item):
        return self.nodes[self.indices[item]]

    def add_node(self, node):
        self.nodes.append(node)
        self.indices[node.name] = len(self.nodes)-1

        if node.observed == False and type(node.distribution) != Delta:
            self.num_params += 1

    def declaration(self):
        s = ""
        for node in self.nodes:
            if not node.observed:
                if type(node.distribution) is Delta:
                    s += node.cpp_type +\
                                " {x};\n".format(x=node.name)
                else:
                    s += node.cpp_type +\
                                " _{x}, {x};\n".format(x=node.name)

        return s

    def from_prior(self):
        s = ""
        for node in self.nodes:
            if node.observed == False and type(node.distribution) is not Delta:
                s += "_{x} = rng.rand();\n".format(x=node.name)
        s += "\n"
        for node in self.nodes:
            if node.observed == False:
                s += "" + node.distribution.from_uniform().format(x=node.name)
        return s

    def perturb(self):
        s = ""
        s += "double logH = 0.0;\n\n"
        s += "int which;\n"
        s += "int reps = 1;\n"
        s += "if(rng.rand() <= 0.5)\n"
        s += "    reps = (int)pow(10.0, 2*rng.rand());\n"
        s += "for(int i=0; i<reps; ++i)\n"
        s += "{\n"
        s += "which = rng.rand_int(" + str(self.num_params) + ");\n"

        k = 0
        for node in self.nodes:
            if node.observed == False and type(node.distribution) != Delta:
                s += "if(which == {k})\n{{\n".format(k=k)
                s += "_{x}".format(x=node.name) + " += rng.randh();\n"
                s += "DNest4::wrap(_{x}, 0.0, 1.0);\n}}\n"\
                                .format(x=node.name);
                k += 1

        s += "}\n\n"
        for node in self.nodes:
            if node.observed == False:
                s += "" + node.distribution.from_uniform()\
                                        .format(x=node.name)

        s += "\nreturn logH;\n\n"
        return s

    def log_likelihood(self):
        s = ""
        s += "double logp = 0.0;\n\n"
        for node in self.nodes:
            if node.observed and type(node.distribution) != Delta:
                
                s += node.distribution.log_prob().format(x=node.name)
        s += "if(std::isnan(logp) || std::isinf(logp))\n"
        s += "    logp = -1E300;\n"
        s += "\nreturn logp;\n"
        return s

    def print(self):
        s = ""
        for node in self.nodes:
            if not node.observed:
                s += "out<<" + node.name + "<<\' \';\n"
        return s

    def description(self):
        s = ""
        s += "return string(\""
        for node in self.nodes:
            if not node.observed:
                s += node.name + ", "
        s = s[0:-2]
        s += "\");"
        return s


class Node:
    """
    Represents a node in the graph.
    """
    def __init__(self, name, distribution, observed=False):
        self.cpp_type = distribution.cpp_type
        self.name = name
        self.distribution = distribution
        self.observed = observed

def data_declaration(data):
    # Static variables for anything which is data or prior info
    s = ""

    for name in data:
        if type(data[name]) == int:
            s += "static constexpr int " + name + ";\n"
        elif type(data[name]) == float:
            s += "static constexpr double " + name + ";\n"
        elif type(data[name] == np.array) and\
            data[name].dtype.name == 'int64':
            for i in range(0, len(data[name])):
                s += "static constexpr int " + name + str(i) + ";\n"
        elif type(data[name] == np.array) and\
            data[name].dtype.name == 'float64':
            for i in range(0, len(data[name])):
                s += "static constexpr double " + name + str(i) + ";\n"

    return s

def data_declaration(data):
    # Static variables for anything which is data or prior info
    s = ""

    for name in data:
        if type(data[name]) == int:
            s += "static constexpr int " + name + " = "\
                                       + str(data[name]) + ";\n"
        elif type(data[name]) == float:
            s += "static constexpr double " + name + " = "\
                                       + str(data[name]) + ";\n"
        elif type(data[name] == np.array) and\
            data[name].dtype.name == 'int64':
            for i in range(0, len(data[name])):
                s += "static constexpr int " + name + str(i) + " = " + str(data[name][i]) + ";\n"
        elif type(data[name] == np.array) and\
            data[name].dtype.name == 'float64':
            for i in range(0, len(data[name])):
                s += "static constexpr double " + name + str(i) + " = " + str(data[name][i]) + ";\n"

    return s

def data_definition(data):
    s = ""
#    # Static variables for anything which is data or prior info
#    for name in data:
#        if type(data[name]) == int:
#            s += "constexpr int MyModel::" + name + " = "\
#                                       + str(data[name]) + ";\n"
#        elif type(data[name]) == float:
#            s += "constexpr double MyModel::" + name + " = "\
#                                       + str(data[name]) + ";\n"
#        elif type(data[name] == np.array) and\
#            data[name].dtype.name == 'int64':
#            for i in range(0, len(data[name])):
#                s += "constexpr int MyModel::" + name + str(i)\
#                                       + " = " + str(data[name][i]) + ";\n"
#        elif type(data[name] == np.array) and\
#            data[name].dtype.name == 'float64':
#            for i in range(0, len(data[name])):
#                s += "constexpr double MyModel::" + name + str(i)\
#                                       + " = " + str(data[name][i]) + ";\n"
    return s

def generate_h(model, data):
    f = open("MyModel.h.template")
    s = "".join(f.readlines())
    f.close()
    s = s.replace("{DECLARATIONS}",
                        model.declaration() + data_declaration(data))
    f = open("MyModel.h", "w")
    f.write(s)
    f.close()

def generate_cpp(model, data):
    f = open("MyModel.cpp.template")
    s = "".join(f.readlines())
    f.close()
    s = s.replace("{STATICS}",
                        data_definition(data))
    s = s.replace("{FROM_PRIOR}",
                        model.from_prior())
    s = s.replace("{PERTURB}",
                        model.perturb())
    s = s.replace("{LOG_LIKELIHOOD}",
                        model.log_likelihood())
    s = s.replace("{PRINT}",
                        model.print())
    s = s.replace("{DESCRIPTION}",
                        model.description())
    f = open("MyModel.cpp", "w")
    f.write(s)
    f.close()




