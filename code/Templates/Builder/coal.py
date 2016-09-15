import numpy as np
import matplotlib.pyplot as plt
import dnest4.builder as bd

# The data, as a dictionary
data = {}
data["t"] = np.array([1851, 1852, 1853, 1854, 1855, 1856, 1857,
                      1858, 1859, 1860, 1861, 1862, 1863, 1864,
                      1865, 1866, 1867, 1868, 1869, 1870, 1871,
                      1872, 1873, 1874, 1875, 1876, 1877, 1878,
                      1879, 1880, 1881, 1882, 1883, 1884, 1885,
                      1886, 1887, 1888, 1889, 1890, 1891, 1892,
                      1893, 1894, 1895, 1896, 1897, 1898, 1899,
                      1900, 1901, 1902, 1903, 1904, 1905, 1906,
                      1907, 1908, 1909, 1910, 1911, 1912, 1913,
                      1914, 1915, 1916, 1917, 1918, 1919, 1920,
                      1921, 1922, 1923, 1924, 1925, 1926, 1927,
                      1928, 1929, 1930, 1931, 1932, 1933, 1934, 
                      1935, 1936, 1937, 1938, 1939, 1940, 1941,
                      1942, 1943, 1944, 1945, 1946, 1947, 1948,
                      1949, 1950, 1951, 1952, 1953, 1954, 1955,
                      1956, 1957, 1958, 1959, 1960, 1961, 1962])\
                                                       .astype("float64")
data["y"] = np.array([4, 5, 4, 1, 0, 4, 3, 4, 0, 6, 3, 3, 4, 0,
                      2, 6, 3, 3, 5, 4, 5, 3, 1, 4, 4, 1, 5, 5,
                      3, 4, 2, 5, 2, 2, 3, 4, 2, 1, 3, 2, 2, 1,
                      1, 1, 1, 3, 0, 0, 1, 0, 1, 1, 0, 0, 3, 1,
                      0, 3, 2, 2, 0, 1, 1, 1, 0, 1, 0, 1, 0, 0,
                      0, 2, 1, 0, 0, 0, 1, 1, 0, 2, 3, 3, 1, 1,
                      2, 1, 1, 1, 1, 2, 4, 2, 0, 0, 0, 1, 4, 0,
                      0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1])\
                                                       .astype("float64")
data["N"] = int(len(data["t"]))

model = bd.Model()
model.add_node(bd.Node("log_mu1", bd.Cauchy(0.0, 5.0)))
model.add_node(bd.Node("diff", bd.Cauchy(0.0, 1.0)))
model.add_node(bd.Node("log_mu2", bd.Delta("log_mu1 + diff")))
model.add_node(bd.Node("mu1", bd.Delta("exp(log_mu1)")))
model.add_node(bd.Node("mu2", bd.Delta("exp(log_mu2)")))
model.add_node(bd.Node("change_year", bd.Uniform(1851.0, 1962.0)))
model.add_node(bd.Node("L", bd.LogUniform(1E-2, 1E2)))

# Data nodes
for i in range(0, data["N"]):
    name = "y{i}".format(i=i)
    mean = "mu1 + (mu2 - mu1)/(1.0 + exp(-(t{i} - change_year)/L))"\
                                        .format(i=i)
    model.add_node(bd.Node(name, bd.Poisson(mean), observed=True))

# Create the C++ code
bd.generate_h(model, data)
bd.generate_cpp(model, data)

# Compile the C++ code so it's ready to go
import os
os.system("make")

