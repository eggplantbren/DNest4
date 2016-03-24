class Uniform:
    def __init__(self, a, b):
        self.a, self.b = a, b

    def from_prior(self):
        s = r"{x} = {a} + ({b} - {a})*rng.rand();\n"
        return self.insert_parameters(s)

    def perturb(self):
        s  = r"{x} += ({b} - {a})*rng.randh();\n"
        s += r"wrap({x}, {a}, {b});\n"
        return self.insert_parameters(s)

    def log_density(self):
        s  = r"if({x} < {a} || {x} > {b})\n"
        s += r"    logp = -numeric_limits<double>::max();\n"
        s += r"logp += -log({b} - {a});\n"
        return self.insert_parameters(s)

    def insert_parameters(self, s):
        s = s.replace("{a}", str(self.a))
        s = s.replace("{b}", str(self.b))
        return s

class Normal:
    def __init__(self, mu, sigma):
        self.mu, self.sigma = mu, sigma

    def from_prior(self):
        s = r"{x} = {mu} + {sigma}*rng.randn();\n"
        return self.insert_parameters(s)

    def perturb(self):
        s  = r"log_H -= -0.5*pow(({x} - {mu})/{sigma}, 2);\n"
        s += r"{x} += {sigma}*rng.randh();\n"
        s += r"log_H += -0.5*pow(({x} - {mu})/{sigma}, 2);\n"
        return self.insert_parameters(s)

    def log_density(self):
        s  = r"logp += -log(2*M_PI) - log({sigma}) "
        s += r"- 0.5*pow(({x} - {mu})/{sigma}, 2);\n"
        return self.insert_parameters(s)

    def insert_parameters(self, s):
        s = s.replace("{mu}", str(self.mu))
        s = s.replace("{sigma}", str(self.sigma))
        return s

class Node:
    def __init__(self, name, prior):
        self.name = name
        self.prior = prior

    def from_prior(self):
        return self.prior.from_prior().replace("{x}", self.name)

    def perturb(self):
        return self.prior.perturb().replace("{x}", self.name)

    def log_density(self):
        return self.prior.log_density().replace("{x}", self.name)

    def __str__(self):
        return self.name

if __name__ == "__main__":
    theta = Node("theta", Uniform(-10.0, 10.0))
    x = Node("x", Normal(theta, 1.0))

    print(x.log_density())

