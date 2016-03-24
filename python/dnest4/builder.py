class Uniform:
    def __init__(self, a, b):
        self.a, self.b = a, b

    def from_prior(self):
        s = "{x} = {a} + ({b} - {a})*rng.rand();"
        return self.insert_parameters(s)

    def insert_parameters(self, s):
        s = s.replace("{a}", str(self.a))
        s = s.replace("{b}", str(self.b))
        return s

class Node:
    def __init__(self, name, prior):
        self.name = name
        self.prior = prior

    def from_prior(self):
        return self.prior.from_prior().replace("{x}", self.name)

    def __str__(self):
        return self.name


if __name__ == "__main__":

    theta = Node("theta", Uniform(0.0, 1.0))

    print(theta.from_prior())

