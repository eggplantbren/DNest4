class Double:
    cpp_type = "double"

class Int:
    cpp_type = "int"

class Uniform(Double):
    """
    Uniform distributions.
    """
    def __init__(self, a, b):
        self.a, self.b = a, b

    def from_uniform(self):
        s = ""
        s += "{x} = {a} + ({b} - ({a}))*_{x};\n"
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

class BiExponential(Double):
    """
    Biexponential distributions
    with location mu, scale s, and skewness r
    """
    def __init__(self, m, s, r):
        self.m, self.s, self.r = m, s, r

    def from_uniform(self):
        s = "double prob_left_{x} = 1.0 / ({r} + 1.0);\n"
        s += "if(_{x} < prob_left_{x})\n"
        s += "{{\n"
        s += "    double u_{x} = _{x}/prob_left_{x};\n"
        s += "    {x} = ({m}) + ({s})/sqrt({r})*log(u_{x});\n"
        s += "}}\n"
        s += "else\n"
        s += "{{\n"
        s += "    double u_{x} = (_{x} - prob_left_{x})/(1.0 - prob_left_{x});\n"
        s += "    {x} = ({m}) - ({s})*sqrt({r})*log(1.0 - u_{x});\n"
        s += "}}\n"
        return self.insert_parameters(s)

    def log_prob(self):
        s = "double logC_{x} = -log({s}) + 0.5*log({r}) - log({r} + 1.0);\n"
        s += "if({x} < ({m}))\n"
        s += "{{\n"
        s += "    logp += logC_{x} + ({x} - ({m}))/(({s})/sqrt({r}));\n"
        s += "}}\n"
        s += "else\n"
        s += "{{\n"
        s += "    logp += logC_{x} - ({x} - ({m}))/(({s})*sqrt({r}));\n"
        s += "}}\n"
        return self.insert_parameters(s)

    def insert_parameters(self, s):
        s = s.replace("{m}", str(self.m))
        s = s.replace("{s}", str(self.s))
        s = s.replace("{r}", str(self.r))
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
        s += "{x} = {mu} + ({sigma})*quantile(__boost_dist, _{x});\n"
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

class Cauchy(Double):
    """
    Cauchy distributions.
    """
    def __init__(self, mu, sigma):
        self.mu, self.sigma = mu, sigma

    def from_uniform(self):
        s = ""
        s += "{x} = {mu} + {sigma}*tan(M_PI*(_{x} - 0.5));\n"
        return self.insert_parameters(s)

    def log_prob(self):
        s = ""
        s += "logp += -log(M_PI) - log({sigma}) "
        s += "-log(1.0 + pow(({x} - {mu})/({sigma}), 2));\n"
        return self.insert_parameters(s)

    def insert_parameters(self, s):
        s = s.replace("{mu}", str(self.mu))
        s = s.replace("{sigma}", str(self.sigma))
        return s

class Binomial(Int):
    """
    Binomial distributions.
    """
    def __init__(self, N, p):
        self.N, self.p = N, p

    def log_prob(self):
        s = ""
        s += "logp += boost::math::lgamma<double>({N} + 1);\n"
        s += "logp -= boost::math::lgamma<double>({x} + 1);\n"
        s += "logp -= boost::math::lgamma<double>({N} - ({x}) + 1);\n"
        s += "logp += ({x})*log({p}) + ({N} - ({x}))*log(1.0 - ({p}));\n"
        return self.insert_parameters(s)

    def insert_parameters(self, s):
        s = s.replace("{N}", str(self.N))
        s = s.replace("{p}", str(self.p))
        return s

class Poisson(Int):
    """
    Poisson distributions.
    """
    def __init__(self, lamb):
        self.lamb = lamb

    def log_prob(self):
        s = ""
        s += "logp += {x}*log({lamb}) - ({lamb});\n"
        s += "logp -= boost::math::lgamma<double>({x} + 1);\n"
        return self.insert_parameters(s)

    def insert_parameters(self, s):
        s = s.replace("{lamb}", str(self.lamb))
        return s

class Gamma(Double):
    """
    Gamma distributions (parameterised by shape and scale)
    """
    def __init__(self, alpha, theta):
        self.alpha, self.theta = alpha, theta

    def from_uniform(self):
        s = ""
        s += "boost::math::gamma_distribution<double> "
        s += "my_gamma_{x}({alpha}, {theta});\n" 
        s += "{x} = quantile(my_gamma_{x}, _{x});\n"
        return self.insert_parameters(s)

    def log_prob(self):
        s = ""
        s += "logp += -({alpha})*log({theta}) "
        s += "- boost::math::lgamma<double>({alpha}) "
        s += "+ ({alpha} - 1.0)*{x} - {x}/({theta});\n"
        return self.insert_parameters(s)

    def insert_parameters(self, s):
        s = s.replace("{alpha}", str(self.alpha))
        s = s.replace("{theta}", str(self.theta))
        return s


class T(Double):
    """
    Student t distributions, parameterised by (location, scale, shape)
    """
    def __init__(self, location, scale, shape):
        self.location = location
        self.scale = scale
        self.shape = shape

    def from_uniform(self):
        s = ""
        s += "boost::math::students_t_distribution<double> "
        s += "my_t_{x}({shape});\n" 
        s += "{x} = {location} + {scale}*quantile(my_t_{x}, _{x});\n"
        return self.insert_parameters(s)

    def log_prob(self):
        s = ""
        s += "logp += -log({scale}) "
        s += "+ boost::math::lgamma<double>(0.5*({shape} + 1.0)) "
        s += "- boost::math::lgamma<double>(0.5*({shape})) "
        s += "- 0.5*log(M_PI*({shape})) "
        s += "- 0.5*({shape} + 1.0)"
        s += "*log(1.0 + pow(({x} - ({location}))/({scale}), 2)/({shape}));\n"
        return self.insert_parameters(s)

    def insert_parameters(self, s):
        s = s.replace("{location}", str(self.location))
        s = s.replace("{scale}", str(self.scale))
        s = s.replace("{shape}", str(self.shape))
        return s




class Delta(Double):
    def __init__(self, formula):
        self.formula = formula

    def from_uniform(self):
        s = "{x} = {formula};\n"
        s = s.replace("{formula}", str(self.formula))
        return s

