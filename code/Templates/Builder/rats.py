import dnest4.builder as bd
import numpy as np
import os

# Data in a dictionary
data = {"N": 30, "T": 5,
            "Y": np.array([151, 145, 147, 155, 135, 159, 141, 159, 177, 134, 160, 143, 154, 171, 163, 160, 142, 156, 157, 152, 154, 139, 146, 
157, 132, 160, 169, 157, 137, 153, 199, 199, 214, 200, 188, 210, 
189, 201, 236, 182, 208, 188, 200, 221, 216, 207, 187, 203, 212, 
203, 205, 190, 191, 211, 185, 207, 216, 205, 180, 200, 246, 249, 
263, 237, 230, 252, 231, 248, 285, 220, 261, 220, 244, 270, 242, 
248, 234, 243, 259, 246, 253, 225, 229, 250, 237, 257, 261, 248, 
219, 244, 283, 293, 312, 272, 280, 298, 275, 297, 350, 260, 313, 
273, 289, 326, 281, 288, 280, 283, 307, 286, 298, 267, 272, 285, 
286, 303, 295, 289, 258, 286, 320, 354, 328, 297, 323, 331, 305, 
338, 376, 296, 352, 314, 325, 358, 312, 324, 316, 317, 336, 321, 
334, 302, 302, 323, 331, 345, 333, 316, 291, 324]).astype("float64"),
            "x": np.array([8.0, 15.0, 22.0, 29.0, 36.0])}
data["x_bar"] = float(data["x"].mean())# Convert from numpy.float64 to float

# A model
model = bd.Model()

# Priors
model.add_node(bd.Node("alpha_mu", bd.Normal(0.0, 100.0)))
model.add_node(bd.Node("beta_mu", bd.Normal(0.0, 100.0)))
model.add_node(bd.Node("tau_c", bd.Gamma(1E-3, 1E3)))
model.add_node(bd.Node("alpha_tau", bd.Gamma(1E-3, 1E3)))
model.add_node(bd.Node("beta_tau", bd.Gamma(1E-3, 1E3)))

# Transformations
model.add_node(bd.Node("alpha_sigma", bd.Delta("1.0/sqrt(alpha_tau)")))
model.add_node(bd.Node("beta_sigma", bd.Delta("1.0/sqrt(beta_tau)")))
model.add_node(bd.Node("sigma_c", bd.Delta("1.0/sqrt(tau_c)")))
model.add_node(bd.Node("a0", bd.Delta("alpha_mu - beta_mu*x_bar")))

# Sampling distribution
for i in range(0, data["N"]):
    model.add_node(bd.Node("n_alpha{i}".format(i=i),\
                        bd.Normal(0.0, 1.0)))
    model.add_node(bd.Node("n_beta{i}".format(i=i),\
                        bd.Normal(0.0, 1.0)))

    name = "alpha{i}".format(i=i)
    value = "alpha_mu + alpha_sigma*n_alpha{i}".format(i=i)
    model.add_node(bd.Node(name, bd.Delta(value)))

    name = "beta{i}".format(i=i)
    value = "beta_mu + beta_sigma*n_beta{i}".format(i=i)
    model.add_node(bd.Node(name, bd.Delta(value)))

    for j in range(0, data["T"]):
        name = "Y{k}".format(k=(data["N"]*j + i))
        mean = "alpha{i} + beta{i}*(x{j} - x_bar)".format(i=i, j=j)
        model.add_node(bd.Node(name, bd.Normal(mean, "sigma_c"),\
                                                    observed=True))

# Create the C++ code
bd.generate_h(model, data)
bd.generate_cpp(model, data)

# Compile the C++ code so it's ready to go
os.system("make")


