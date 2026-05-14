#!/usr/bin/env python3
import sys
import json
import numpy as np

num_params = 10

def prior_transform(us):
    return us - 0.5

def log_likelihood(params):
    return -num_params*0.5*np.log(2*np.pi*0.01**2) - 0.5*np.sum((params/0.01)**2)

def both(us):
    return log_likelihood(prior_transform(us))

# --- JSON IPC loop ---
for line in sys.stdin:
    req = json.loads(line)

    t = req.get("type")

    if t == "loglik":
        params = np.array(req["params"])
        out = {"loglik": float(log_likelihood(params))}

    elif t == "prior":
        us = np.array(req["us"])
        params = prior_transform(us)
        params = params.astype(float)   # convert NumPy float64 → Python float
        out = {"params": params.tolist()}


    elif t == "num_params":
        out = {"num_params": num_params}

    else:
        out = {"error": f"Unknown request type: {t}"}

    sys.stdout.write(json.dumps(out, default=repr) + "\n")
    sys.stdout.flush()

