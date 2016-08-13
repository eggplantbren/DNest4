import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

# Import DNest4's builder
import dnest4.builder as bd

# Load the data and make a dictionary out of it
nzunemployment = pd.read_csv("nzunemployment.csv")
data = {}
data["N"] = nzunemployment.shape[0]
data["t"] = np.array(nzunemployment["q"])
data["adult"] = np.array(nzunemployment["adult"])
data["youth"] = np.array(nzunemployment["youth"])

# Convert to logits of unemployment rates
data["adult"] *= 0.01
data["adult"] = np.log(data["adult"]/(1.0 - data["adult"]))
data["youth"] *= 0.01
data["youth"] = np.log(data["youth"]/(1.0 - data["youth"]))

# Plot the data
plt.plot(data["t"], data["adult"], "ko-", label="Adult")
plt.plot(data["t"], data["youth"], "go-", label="Youth")
plt.xlabel("Time (quarters)")
plt.ylabel("Unemployment rate (logit)")
plt.legend()
plt.xlim([data["t"].min() - 0.5, data["t"].max() + 0.5])
plt.ylim([-4.0, 0.0])
plt.show()

# A model (of the prior information!)
model = bd.Model()

# AR(1) parameters for adult unemployment rate
model.add_node(bd.Node("mu1", bd.Uniform(-10.0, 0.0)))
model.add_node(bd.Node("L1", bd.LogUniform(1.0, 1E4)))
model.add_node(bd.Node("beta1", bd.LogUniform(1E-3, 1E3)))
model.add_node(bd.Node("alpha1", bd.Delta("exp(-1.0/L1)")))
model.add_node(bd.Node("sigma1", bd.Delta("beta1/sqrt(1.0 - alpha1*alpha1)")))

# Sampling distribution for adult data
model.add_node(bd.Node("adult0",\
                    bd.Normal("mu1", "sigma1"), observed=True))
for i in range(1, data["N"]):
    name = "adult{i}".format(i=i)
    dist = bd.Normal("mu1 + alpha1*(adult{k} - mu1)".format(k=(i-1)), "beta1")
    model.add_node(bd.Node(name, dist, observed=True))

# Parameters relating to youth data
model.add_node(bd.Node("offset", bd.Normal(0.0, 1.0)))
model.add_node(bd.Node("policy_effect", bd.Cauchy(0.0, 0.1)))
model.add_node(bd.Node("L2", bd.LogUniform(1E-2, 1E2)))
model.add_node(bd.Node("beta2", bd.LogUniform(1E-3, 1E3)))
model.add_node(bd.Node("alpha2", bd.Delta("exp(-1.0/L2)")))
model.add_node(bd.Node("sigma2", bd.Delta("beta2/sqrt(1.0 - alpha2*alpha2)")))

for i in range(0, data["N"]):
    name = "youth{i}".format(i=i)
    mean = "adult{i} + offset + ((t{i} >= 90.0)?(policy_effect):(0.0))"\
                                                                .format(i=i)
    sd = "sigma2"
    if i > 0:
        ar1_mean = "{mean} + alpha2*(youth{k} - ({mean}))".format(mean=mean,\
                                    k=(i-1))
        mean = ar1_mean
        sd = "beta2"

    dist = bd.Normal(mean, sd)
    model.add_node(bd.Node(name, dist, observed=True))

# Create the C++ code
bd.generate_h(model, data)
bd.generate_cpp(model, data)

# Compile the C++ code so it's ready to go
import os
os.system("make")

