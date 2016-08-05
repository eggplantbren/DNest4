class Double:
    cpp_type = "double"

class Int:
    cpp_type = "int"

class Uniform(Double):
    def __init__(self, a, b):
        self.a, self.b = a, b

    def from_uniform(self):
        s = ""
        s += "{x} = a + (b - a)*_{x};\n"
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


class LogUniform(Double):
    """
    Log-uniform distributions.
    """
    def __init__(self, a, b):
        self.a, self.b = a, b

    def from_uniform(self):
        s = ""
        s += "{x} = exp(log({a}) + log(({b})/({a}))*_{x});\n"
        return self.insert_parameters(s)

    def log_prob(self):
        s  = "if({x} < ({a}) || {x} > ({b}))\n"
        s += "    logp = -numeric_limits<double>::max();\n"
        s += "logp += -log({x}) - log(log(({b})/({a})));\n"
        return self.insert_parameters(s)

    def insert_parameters(self, s):
        s = s.replace("{a}", str(self.a))
        s = s.replace("{b}", str(self.b))
        return s

class Exponential(Double):
    """
    Exponential distributions.
    """
    def __init__(self, mu):
        self.mu = mu

    def from_uniform(self):
        s = ""
        s += "{x} = -({mu})*log(1.0 - _{x});\n"
        return self.insert_parameters(s)

    def log_prob(self):
        s  = "if({x} < 0.0)\n"
        s += "    logp = -numeric_limits<double>::max();\n"
        s += "logp += -log({mu}) - {x}/({mu});\n"
        return self.insert_parameters(s)

    def insert_parameters(self, s):
        s = s.replace("{mu}", str(self.mu))
        return s

class Normal(Double):
    """
    Normal distributions.
    """
    def __init__(self, mu, sigma):
        self.mu, self.sigma = mu, sigma

    def from_uniform(self):
        s = ""
        s += "{x} = {mu} + {sigma}*quantile(__boost_dist, _{x});\n"
        return self.insert_parameters(s)

    def log_prob(self):
        s = ""
        s += "logp += -0.5*log(2*M_PI) - log({sigma}) "
        s += "- 0.5*pow((({x}) - ({mu}))/({sigma}), 2);\n"
        return self.insert_parameters(s)

    def insert_parameters(self, s):
        s = s.replace("{mu}", str(self.mu))
        s = s.replace("{sigma}", str(self.sigma))
        return s

class Delta(Double):
    def __init__(self, formula):
        self.formula = formula

    def from_uniform(self):
        s = "{x} = {formula};\n"
        s = s.replace("{formula}", str(self.formula))
        return s

