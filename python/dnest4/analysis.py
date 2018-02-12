# -*- coding: utf-8 -*-

import numpy as np

from .backends import CSVBackend

try:
    basestring
except NameError:
    stringtype = str
else:
    stringtype = basestring

__all__ = ["postprocess", "make_plots"]


def postprocess(backend=None,
                temperature=1.0, cut=0, compression_assert=None,
                resample_log_X=0,
                compression_bias_min=1, compression_scatter=0,
                resample=0,
                plot=False, plot_params=None):
    # Deal with filename inputs.
    if backend is None:
        backend = "."
    if isinstance(backend, stringtype):
        backend = CSVBackend(backend)

    # Unpack the backend's data.
    levels = backend.levels
    samples = backend.samples
    sample_info = backend.sample_info

    # Remove regularisation from levels if we asked for it.
    if compression_assert is not None:
        levels = np.array(levels)
        levels["log_X"][1:] = \
            -np.cumsum(compression_assert*np.ones(len(levels) - 1))

    # Remove burn-in.
    if cut > 0:
        samples, sample_info = remove_burnin(samples, sample_info, cut)

    # Subsample; one (randomly selected) particle for each time.
    if len(sample_info.shape) > 1:
        samples, sample_info = subsample_particles(samples, sample_info)

    # Check dimensions.
    assert len(samples) == len(sample_info), "dimension mismatch"

    # Estimate the X values for the samples by interpolating from the levels.
    if resample_log_X:
        resample_count = resample_log_X
    else:
        resample_count = 1

    log_z = np.empty(resample_count)
    h = np.empty(resample_count)
    n_eff = np.empty(resample_count)
    log_post = np.empty((resample_count, len(sample_info)))
    for i in range(resample_count):
        # If requested, jitter the Xs of the levels.
        if resample_log_X:
            levels_2 = np.array(levels)
            comp = -np.diff(levels_2["log_X"])
            comp *= np.random.uniform(compression_bias_min, 1.0)
            comp *= np.exp(compression_scatter*np.random.randn(len(comp)))
            levels_2["log_X"][1:] = -comp
            levels_2["log_X"] = np.cumsum(levels_2["log_X"])
        else:
            levels_2 = levels

        sample_log_X = interpolate_samples(levels_2, sample_info,
                                           resample=resample_log_X)
        if i == 0:
            backend.write_sample_log_X(sample_log_X)
        log_z[i], h[i], n_eff[i], log_post[i] = compute_stats(
            levels_2, sample_info, sample_log_X,
            temperature=temperature,
        )

    # Re-sample the samples using the posterior weights.
    log_post = logsumexp(log_post, axis=0) - np.log(resample_count)
    backend.write_weights(np.exp(log_post))
    if resample:
        new_samples = generate_posterior_samples(
            samples, log_post, int(resample * np.mean(n_eff))
        )
        backend.write_posterior_samples(new_samples)
        log_post = np.zeros(len(new_samples))
    else:
        new_samples = samples

    # Compute the final stats based on resampling.
    stats = dict(
        log_Z=np.mean(log_z), log_Z_std=np.std(log_z),
        H=np.mean(h), H_std=np.std(h),
        N_eff=np.mean(n_eff), N_eff_std=np.std(n_eff),
    )
    backend.write_stats(stats)

    # Make the plots if requested.
    if plot:
        if plot_params is None:
            plot_params = dict()
        make_plots(backend, **plot_params)

    return stats


def logsumexp(x, axis=None):
    mx = np.max(x, axis=axis)
    return np.log(np.sum(np.exp(x - mx), axis=axis)) + mx


def remove_burnin(samples, sample_info, nburn):
    return (
        samples[int(nburn * len(samples)):],
        sample_info[int(nburn * len(sample_info)):],
    )


def subsample_particles(samples, sample_info):
    if len(samples.shape) == 2 and len(sample_info.shape) == 1:
        return samples, sample_info

    if len(sample_info.shape) != 2:
        raise ValueError("invalid dimensions")

    # Subsample; one (randomly selected) particle for each time.
    if samples.shape[1] != sample_info.shape[1]:
        raise ValueError("dimension mismatch")

    n = np.prod(sample_info.shape)
    return samples.reshape((n, -1)), sample_info.reshape(n)

    # inds = (
    #     np.arange(len(samples)),
    #     np.random.randint(samples.shape[1], size=len(samples)),
    # )
    # return samples[inds], sample_info[inds]


def interpolate_samples(levels, sample_info, resample=False):
    # Work out the level assignments. This looks horrifying because we need
    # to take tiebreakers into account; if two levels (or samples) have
    # exactly the same likelihood, then the tiebreaker decides the assignment.
    lev, order = 0, 0
    assign = np.empty(len(sample_info), dtype=int)
    argsort = np.empty(len(sample_info), dtype=int)
    l_set = zip(levels["log_likelihood"], levels["tiebreaker"],
                -np.arange(1, len(levels)+1))
    s_set = zip(sample_info["log_likelihood"], sample_info["tiebreaker"],
                range(len(sample_info)))
    for ll, _, ind in sorted(list(l_set) + list(s_set)):
        if ind < 0:
            lev = -ind - 1
            continue
        assign[ind] = lev
        argsort[ind] = order
        order += 1

    # Loop over levels and place the samples within each level.
    sample_log_X = np.empty(len(sample_info))
    x_min = np.exp(np.append(levels["log_X"][1:], -np.inf))
    x_max = np.exp(levels["log_X"])
    dx = x_max - x_min
    for i, lev in enumerate(levels):
        # Use the level assignments to get a mask of sample IDs in the correct
        # order.
        m = assign == i
        inds = np.arange(len(sample_info))[m][np.argsort(argsort[m])]

        if resample:
            # Re-sample the points uniformly---in X---between the level
            # boundaries.
            sample_log_X[inds] = np.sort(np.log(
                np.random.uniform(x_min[i], x_max[i], size=len(inds))
            ))[::-1]
        else:
            # Place the samples uniformly---in X not log(X)---between the
            # level boundaries.
            N = len(inds)

            # FIXME: there are two options here and we're using the backwards
            # compatible one but the other might be better. Need to think
            # about it further. It won't matter as the number of samples gets
            # large.
            n = ((np.arange(1, N+1)) / (N+1))[::-1]
            # n = ((np.arange(N) + 0.5) / N)[::-1]

            sample_log_X[inds] = np.log(x_min[i] + dx[i] * n)

    return sample_log_X


def compute_stats(levels, sample_info, sample_log_X, temperature=1.0):
    # Use the log(X) estimates for the levels and the samples to estimate
    # log(Z) using the trapezoid rule.
    log_x = np.append(levels["log_X"], sample_log_X)
    log_y = np.append(levels["log_likelihood"], sample_info["log_likelihood"])
    samp_inds = np.append(-np.ones(len(levels), dtype=int),
                          np.arange(len(sample_info)))
    is_samp = np.append(
        np.zeros(len(levels), dtype=bool),
        np.ones(len(sample_info), dtype=bool)
    )
    inds = np.argsort(log_x)
    log_x = log_x[inds]
    log_y = log_y[inds] / temperature
    samp_inds = samp_inds[inds]
    is_samp = is_samp[inds]

    # Extend to X=0.
    log_x = np.append(-np.inf, log_x)
    log_y = np.append(log_y[0], log_y)

    # Compute log(exp(L_k+1) + exp(L_k)) using logsumexp rules...
    d_log_y = log_y[1:] - log_y[:-1]
    log_y_mean = np.log(0.5) + np.log(1+np.exp(d_log_y)) + log_y[:-1]

    # ...and log(exp(log(X_k+1)) + exp(log(X_k))) using logsumexp rules.
    log_x_diff = np.log(1. - np.exp(log_x[:-1] - log_x[1:])) + log_x[1:]

    # Then from the trapezoid rule:
    #   log(Z) = log(0.5) + logsumexp(log_x_diff + log_y_mean)
    log_p = log_x_diff + log_y_mean
    log_z = logsumexp(log_p)
    log_p -= log_z

    # Compute the sample posterior weights. These are equal to:
    #   w_k = L_k / (0.5 * (X_k+1 - X_k-1)) / Z
    # but we'll recompute Z not using the levels just to be safe.
    log_prior = np.log(0.5) + np.logaddexp(log_x_diff[1:], log_x_diff[:-1])
    log_post = np.array(sample_info["log_likelihood"])
    log_post[samp_inds[samp_inds >= 0]] += log_prior[samp_inds[:-1] >= 0]
    log_post -= logsumexp(log_post)

    # Compute the information and effective sample size.
    h = -log_z + np.sum(np.exp(log_post) * sample_info["log_likelihood"])
    n_eff = np.exp(-np.sum(np.exp(log_post)*log_post))

    return log_z, h, n_eff, log_post


def generate_posterior_samples(samples, log_weights, N):
    w = np.exp(log_weights - logsumexp(log_weights))
    inds = np.random.choice(np.arange(len(samples)), size=int(N), p=w)
    return samples[inds]


def make_plots(backend):
    figs = dict()

    figs["levels"] = make_levels_plot(backend)
    figs["compression"] = make_compression_plot(backend)
    figs["log_X_log_L"] = make_log_X_log_L_plot(backend)

    return figs


def make_levels_plot(backend):
    import matplotlib.pyplot as pl
    fig, ax = pl.subplots(1, 1)

    ax.plot(backend.sample_info["level_assignment"], color="k")
    ax.set_xlabel("Iterations")
    ax.set_ylabel("Level")

    return fig


def make_compression_plot(backend):
    import matplotlib.pyplot as pl
    fig, axes = pl.subplots(2, 1, sharex=True)

    levels = backend.levels

    ax = axes[0]
    ax.plot(np.diff(levels["log_X"]), color="k")
    ax.axhline(-1., color="g")
    ax.axhline(-np.log(10.), color="g", linestyle="--")
    ax.set_ylim(ymax=0.05)
    ax.set_ylabel("Compression")

    ax = axes[1]
    m = levels["tries"] > 0
    ax.plot(np.arange(len(levels))[m],
            levels[m]["accepts"]/levels[m]["tries"],
            "ko-")
    ax.set_ylabel("MH Acceptance")
    ax.set_xlabel("level")
    ax.set_ylim([0.0, 1.0])

    return fig


def make_log_X_log_L_plot(backend):
    import matplotlib.pyplot as pl
    fig, axes = pl.subplots(2, 1, sharex=True)

    levels = backend.levels
    sample_info = backend.sample_info
    sample_log_X = backend.sample_log_X
    weights = backend.weights

    ax = axes[0]
    ax.plot(sample_log_X.flatten(), sample_info["log_likelihood"].flatten(),
            "k.", label="Samples")
    ax.plot(levels["log_X"][1:], levels["log_likelihood"][1:], "g.",
            label="Levels")
    ax.legend(numpoints=1, loc="lower left")
    ax.set_ylabel("log(L)")
    ax.set_title("log(Z) = {0}".format(backend.stats["log_Z"]))

    # Use all plotted logl values to set ylim
    combined_logl = np.hstack([sample_info["log_likelihood"],\
                                levels["log_likelihood"][1:]])
    combined_logl = np.sort(combined_logl)
    lower = combined_logl[int(0.1*combined_logl.size)]
    upper = combined_logl[-1]
    diff = upper - lower
    lower -= 0.05*diff
    upper += 0.05*diff
    ax.set_ylim([lower, upper])

    ax = axes[1]
    ax.plot(sample_log_X, weights, "k.")
    ax.set_ylabel("posterior weight")
    ax.set_xlabel("log(X)")

    return fig
