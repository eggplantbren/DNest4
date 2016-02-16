# -*- coding: utf-8 -*-

import numpy as np
import matplotlib.pyplot as pl

__all__ = ["postprocess"]


def remove_burnin(samples, sample_info, nburn):
    return (
        samples[int(nburn * len(samples)):, :],
        sample_info[int(nburn * len(sample_info)):, :],
    )


def subsample_particles(samples, sample_info):
    if len(sample_info.shape) != 2:
        raise ValueError("invalid dimensions")

    # Subsample; one (randomly selected) particle for each time.
    if samples.shape[1] != sample_info.shape[1]:
        raise ValueError("dimension mismatch")

    inds = (
        np.arange(len(samples)),
        np.random.randint(samples.shape[1], size=len(samples)),
    )

    return samples[inds], sample_info[inds]


def interpolate_samples(levels, sample_info, resample=None):
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

        if resample is not None:
            # Re-sample the points uniformly---in X---between the level
            # boundaries.
            sample_log_X[inds] = np.sort(np.log(
                np.random.uniform(x_min[i], x_max[i], size=len(inds))
            ))[::-1]
        else:
            # Place the samples uniformly---in X not log(X)---between the
            # level boundaries.
            N = len(inds)
            n = ((np.arange(N, 0, -1) - 0.5) / N)
            sample_log_X[inds] = np.log(x_min[i] + dx[i] * n)

    return sample_log_X


def compute_stats(levels, sample_info, sample_log_X, temperature=1.0):
    # pl.plot(sample_log_X, sample_info["log_likelihood"], ".b")
    # pl.plot(levels["log_X"][1:], levels["log_likelihood"][1:], ".r")
    # pl.savefig("levels.png")

    # Use the log(X) estimates for the levels and the samples to estimate
    # log(Z) using the trapezoid rule.
    log_x = np.append(levels["log_X"], sample_log_X)
    log_y = np.append(levels["log_likelihood"], sample_info["log_likelihood"])
    inds = np.argsort(log_x)
    log_x = log_x[inds]
    log_y = log_y[inds] / temperature

    # # Extend the integration region to X = 0.
    # log_x = np.append(-np.inf, log_x)
    # log_y = np.append(log_y[0], log_y)

    # Compute log(exp(L_k+1) + exp(L_k)) using logsumexp rules...
    d_log_y = log_y[1:] - log_y[:-1]
    log_y_mean = np.log(0.5) + np.log(1+np.exp(d_log_y)) + log_y[:-1]

    # ...and log(exp(log(X_k+1)) + exp(log(X_k))) using logsumexp rules.
    log_x_diff = np.log(1. - np.exp(log_x[:-1] - log_x[1:])) + log_x[1:]

    # TODO: should this be normalized to 1?
    # It probably doesn't change the results...
    # log_x_diff -= logsumexp(log_x_diff)

    # Then from the trapezoid rule:
    #   log(Z) = log(0.5) + logsumexp(log_x_diff + log_y_mean)
    log_p = log_x_diff + log_y_mean
    log_z = logsumexp(log_p)
    log_p -= log_z
    h = -log_z + np.sum(np.exp(log_p) * log_y_mean)
    n_eff = np.exp(-np.sum(np.exp(log_p)*log_p))
    return dict(
        log_Z=log_z,
        H=h,
        N_eff=n_eff,
    )


def postprocess(levels, samples, sample_info, compression_assert=None, cut=0,
                temperature=1.0, plot=True,
                resample=1, compression_bias_min=1., compression_scatter=0.):
    # Remove regularisation from levels if we asked for it.
    if compression_assert is not None:
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

    sample_log_X = interpolate_samples(levels, sample_info)

    return compute_stats(
        levels, sample_info, sample_log_X,
        temperature=temperature,
    )

    assert 0

    # Find the correct level assignments.
    assign = np.digitize(sample_info["log_likelihood"],
                         levels["log_likelihood"]) - 1
    assert np.all(assign >= 0), "some samples not within level bounds"

    # Some initial plotting.
    if plot:
        fig, ax = pl.subplots(1, 1)
        ax.plot(assign)
        ax.set_xlabel("Iteration")
        ax.set_ylabel("Level")
        fig.savefig("levels_iteration.png")
        pl.close(fig)

        fig, ax = pl.subplots(1, 1)
        ax.plot(np.diff(levels["log_X"]))
        ax.set_ylabel("Compression")
        ax.set_xlabel("Level")
        ax.axhline(-1., color='r')
        ax.axhline(-np.log(10.), color='g')
        ax.set_ylim(ymax=0.05)
        fig.savefig("compression.png")
        pl.close(fig)

    # Bookkeeping.
    shape = (len(sample_info), resample)
    logx_samp = np.empty(shape)
    logp_samp = np.empty(shape)
    logP_samp = np.empty(shape)
    P_samp = np.empty(shape)
    logz_est = np.empty((resample, 1))
    H_est = np.empty((resample, 1))

    # Make a Monte Carlo perturbation to the level compressions.
    for z in range(resample):
        levs = np.array(levels)
        comp = -np.diff(levs["log_X"])
        comp *= compression_bias_min+(1-compression_bias_min)*np.random.rand()
        comp *= np.exp(compression_scatter*np.random.randn(len(comp)))
        levs["log_X"][1:] = -comp
        levs["log_X"] = np.cumsum(levs["log_X"])

        for i, lev in enumerate(levs):
            # Get all the samples in this level.
            inds = np.arange(len(assign))[assign == i]
            logl_lev = sample_info[inds]
            m = np.argsort(logl_lev["log_likelihood"])
            logl_lev = logl_lev[m]
            inds = inds[m]
            N = len(logl_lev)

            # Intermediate log(X) values.
            logx_max = lev["log_X"]
            logx_min = -1e300 if i == len(levs)-1 else levs["log_X"][i+1]
            Umin = np.exp(logx_min - logx_max)
            if N == 0 or resample > 1:
                U = np.random.uniform(Umin, 1.0, size=max(N, 1))
            else:
                oon = 1.0 / (N + 1.0)
                U = Umin + (1-Umin)*np.linspace(oon, 1-oon, N)
            logx_lev = np.sort(logx_max + np.log(U))[::-1]

            # Update the estimates of everything.
            logx_samp[inds, z] = logx_lev

            left = -1e300 if i == len(levs)-1 else levs["log_X"][i+1]
            right = np.append(lev["log_X"], logx_lev[:-1])
            logp_samp[inds, z] = \
                np.log(0.5) + np.log(1.0-np.exp(left-right)) + right

        logl = sample_info["log_likelihood"] / temperature
        logp_samp[:, z] -= logsumexp(logp_samp[:, z])
        logP_samp[:, z] = logp_samp[:, z] + logl
        logz_est[z] = logsumexp(logP_samp[:, z])
        logP_samp[:, z] -= logz_est[z]
        P_samp[:, z] = np.exp(logP_samp[:, z])
        H_est[z] = -logz_est[z] + np.sum(P_samp[:, z]*logl)

        if plot:
            fig, ax = pl.subplots(1, 1)

            ax.plot(logx_samp[:, z], sample_info["log_likelihood"], 'b.',
                    label='Samples')
            ax.plot(levs["log_X"][1:], levs["log_likelihood"][1:], 'r.',
                    label='Levels')
            ax.legend(numpoints=1, loc='lower left')
            ax.set_ylabel('log(L)')
            ax.set_xlabel('log(X)')
            # ax.set_title(str(z+1) + "/" + str(numResampleLogX) + ", log(Z) = " + str(logz_estimates[z][0]))

            fig.savefig("log_X.png")
            pl.close(fig)

    P_samp = np.mean(P_samp, axis=1)
    P_samp = P_samp / np.sum(P_samp)
    logz_est = np.mean(logz_est)
    logz_error = np.std(logz_est)
    H_est = np.mean(H_est)
    H_error = np.std(H_est)
    ESS = np.exp(-np.sum(P_samp*np.log(P_samp+1E-300)))

    print("log(Z) = " + str(logz_est) + " +- " + str(logz_error))
    print("Information = " + str(H_est) + " +- " + str(H_error) + " nats.")
    print("Effective sample size = " + str(ESS))
    assert 0


def logsumexp(x):
    mx = np.max(x)
    result = np.log(np.sum(np.exp(x - mx))) + mx
    return result

# import copy
# import numpy as np
# import matplotlib.pyplot as plt
#
# def logsumexp(values):
# 	biggest = np.max(values)
# 	x = values - biggest
# 	result = np.log(np.sum(np.exp(x))) + biggest
# 	return result
#
# def logdiffexp(x1, x2):
# 	biggest = x1
# 	xx1 = x1 - biggest
# 	xx2 = x2 - biggest
# 	result = np.log(np.exp(xx1) - np.exp(xx2)) + biggest
# 	return result
#
# def postprocess(temperature=1., numResampleLogX=1, plot=True, loaded=[], \
# 			cut=0., save=True, zoom_in=True, compression_bias_min=1., compression_scatter=0., moreSamples=1., compression_assert=None):
#
# 	if plot:
# 		if numResampleLogX > 1:
# 			plt.ion()
#
# 		plt.figure(1)
# 		plt.plot(sample_info[:,0])
# 		plt.xlabel("Iteration")
# 		plt.ylabel("Level")
# 		if numResampleLogX > 1:
# 			plt.draw()
#
# 		plt.figure(2)
# 		plt.subplot(2,1,1)
# 		plt.plot(np.diff(levels_orig[:,0]))
# 		plt.ylabel("Compression")
# 		plt.xlabel("Level")
# 		xlim = plt.gca().get_xlim()
# 		plt.axhline(-1., color='r')
# 		plt.axhline(-np.log(10.), color='g')
# 		plt.ylim(ymax=0.05)
# 		if numResampleLogX > 1:
# 			plt.draw()
#
# 		plt.subplot(2,1,2)
# 		good = np.nonzero(levels_orig[:,4] > 0)[0]
# 		plt.plot(levels_orig[good,3]/levels_orig[good,4])
# 		plt.xlim(xlim)
# 		plt.ylim([0., 1.])
# 		plt.xlabel("Level")
# 		plt.ylabel("MH Acceptance")
# 		if numResampleLogX > 1:
# 			plt.draw()
#
# 	# Convert to lists of tuples
# 	logl_levels = [(levels_orig[i,1], levels_orig[i, 2]) for i in range(0, levels_orig.shape[0])] # logl, tiebreaker
# 	logl_samples = [(sample_info[i, 1], sample_info[i, 2], i) for i in range(0, sample.shape[0])] # logl, tiebreaker, id
# 	logx_samples = np.zeros((sample_info.shape[0], numResampleLogX))
# 	logp_samples = np.zeros((sample_info.shape[0], numResampleLogX))
# 	logP_samples = np.zeros((sample_info.shape[0], numResampleLogX))
# 	P_samples = np.zeros((sample_info.shape[0], numResampleLogX))
# 	logz_estimates = np.zeros((numResampleLogX, 1))
# 	H_estimates = np.zeros((numResampleLogX, 1))
#
# 	# Find sandwiching level for each sample
# 	sandwich = sample_info[:,0].copy().astype('int')
# 	for i in range(0, sample.shape[0]):
# 		while sandwich[i] < levels_orig.shape[0]-1 and logl_samples[i] > logl_levels[sandwich[i] + 1]:
# 			sandwich[i] += 1
#
# 	for z in range(0, numResampleLogX):
# 		# Make a monte carlo perturbation of the level compressions
# 		levels = levels_orig.copy()
# 		compressions = -np.diff(levels[:,0])
# 		compressions *= compression_bias_min + (1. - compression_bias_min)*np.random.rand()
# 		compressions *= np.exp(compression_scatter*np.random.randn(compressions.size))
# 		levels[1:, 0] = -compressions
# 		levels[:, 0] = np.cumsum(levels[:,0])
#
# 		# For each level
# 		for i in range(0, levels.shape[0]):
# 			# Find the samples sandwiched by this level
# 			which = np.nonzero(sandwich == i)[0]
# 			logl_samples_thisLevel = [] # (logl, tieBreaker, ID)
# 			for j in range(0, len(which)):
# 				logl_samples_thisLevel.append(copy.deepcopy(logl_samples[which[j]]))
# 			logl_samples_thisLevel = sorted(logl_samples_thisLevel)
# 			N = len(logl_samples_thisLevel)
#
# 			# Generate intermediate logx values
# 			logx_max = levels[i, 0]
# 			if i == levels.shape[0]-1:
# 				logx_min = -1E300
# 			else:
# 				logx_min = levels[i+1, 0]
# 			Umin = np.exp(logx_min - logx_max)
#
# 			if N == 0 or numResampleLogX > 1:
# 				U = Umin + (1. - Umin)*np.random.rand(len(which))
# 			else:
# 				U = Umin + (1. - Umin)*np.linspace(1./(N+1), 1. - 1./(N+1), N)
# 			logx_samples_thisLevel = np.sort(logx_max + np.log(U))[::-1]
#
# 			for j in range(0, which.size):
# 				logx_samples[logl_samples_thisLevel[j][2]][z] = logx_samples_thisLevel[j]
#
# 				if j != which.size - 1:
# 					left = logx_samples_thisLevel[j+1]
# 				elif i == levels.shape[0]-1:
# 					left = -1E300
# 				else:
# 					left = levels[i+1][0]
#
# 				if j != 0:
# 					right = logx_samples_thisLevel[j-1]
# 				else:
# 					right = levels[i][0]
#
# 				logp_samples[logl_samples_thisLevel[j][2]][z] = np.log(0.5) + logdiffexp(right, left)
#
# 		logl = sample_info[:,1]/temperature
#
# 		logp_samples[:,z] = logp_samples[:,z] - logsumexp(logp_samples[:,z])
# 		logP_samples[:,z] = logp_samples[:,z] + logl
# 		logz_estimates[z] = logsumexp(logP_samples[:,z])
# 		logP_samples[:,z] -= logz_estimates[z]
# 		P_samples[:,z] = np.exp(logP_samples[:,z])
# 		H_estimates[z] = -logz_estimates[z] + np.sum(P_samples[:,z]*logl)
#
# 		if plot:
# 			plt.figure(3)
#
# 			plt.subplot(2,1,1)
# 			plt.hold(False)
# 			plt.plot(logx_samples[:,z], sample_info[:,1], 'b.', label='Samples')
# 			plt.hold(True)
# 			plt.plot(levels[1:,0], levels[1:,1], 'r.', label='Levels')
# 			plt.legend(numpoints=1, loc='lower left')
# 			plt.ylabel('log(L)')
# 			plt.title(str(z+1) + "/" + str(numResampleLogX) + ", log(Z) = " + str(logz_estimates[z][0]))
# 			# Use all plotted logl values to set ylim
# 			combined_logl = np.hstack([sample_info[:,1], levels[1:, 1]])
# 			combined_logl = np.sort(combined_logl)
# 			lower = combined_logl[int(0.1*combined_logl.size)]
# 			upper = combined_logl[-1]
# 			diff = upper - lower
# 			lower -= 0.05*diff
# 			upper += 0.05*diff
# 			if zoom_in:
# 				plt.ylim([lower, upper])
#
# 			if numResampleLogX > 1:
# 				plt.draw()
# 			xlim = plt.gca().get_xlim()
#
# 		if plot:
# 			plt.subplot(2,1,2)
# 			plt.hold(False)
# 			plt.plot(logx_samples[:,z], P_samples[:,z], 'b.')
# 			plt.ylabel('Posterior Weights')
# 			plt.xlabel('log(X)')
# 			plt.xlim(xlim)
# 			if numResampleLogX > 1:
# 				plt.draw()
#
# 	P_samples = np.mean(P_samples, 1)
# 	P_samples = P_samples/np.sum(P_samples)
# 	logz_estimate = np.mean(logz_estimates)
# 	logz_error = np.std(logz_estimates)
# 	H_estimate = np.mean(H_estimates)
# 	H_error = np.std(H_estimates)
# 	ESS = np.exp(-np.sum(P_samples*np.log(P_samples+1E-300)))
#
# 	print("log(Z) = " + str(logz_estimate) + " +- " + str(logz_error))
# 	print("Information = " + str(H_estimate) + " +- " + str(H_error) + " nats.")
# 	print("Effective sample size = " + str(ESS))
#
# 	# Resample to uniform weight
# 	N = int(moreSamples*ESS)
# 	posterior_sample = np.zeros((N, sample.shape[1]))
# 	w = P_samples
# 	w = w/np.max(w)
# 	if save:
# 		np.savetxt('weights.txt', w) # Save weights
# 	for i in range(0, N):
# 		while True:
# 			which = np.random.randint(sample.shape[0])
# 			if np.random.rand() <= w[which]:
# 				break
# 		posterior_sample[i,:] = sample[which,:]
# 	if save:
# 		np.savetxt("posterior_sample.txt", posterior_sample)
#
# 	if plot:
# 		if numResampleLogX > 1:
# 			plt.ioff()
# 		plt.show()
#
# 	return [logz_estimate, H_estimate, logx_samples]
#
#
# def diffusion_plot():
# 	"""
# 	Plot a nice per-particle diffusion plot.
# 	"""
#
# 	sample_info = np.atleast_2d(np.loadtxt('sample_info.txt'))
# 	ID = sample_info[:,3].astype('int')
# 	j = sample_info[:,0].astype('int')
#
# 	ii = np.arange(1, sample_info.shape[0] + 1)
#
# 	for i in range(0, ID.max() + 1):
# 		which = np.nonzero(ID == i)[0]
# 		plt.plot(ii[which], j[which])
#
# 	plt.xlabel('Iteration')
# 	plt.ylabel('Level')
# 	plt.show()
#
# def levels_plot():
# 	"""
# 	Plot the differences between the logl values of the levels.
# 	"""
# 	levels = np.loadtxt('levels.txt')
#
# 	plt.plot(np.log10(np.diff(levels[:,1])))
# 	plt.ylim([-1, 4])
# 	plt.axhline(0., color='r')
# 	plt.axhline(np.log10(np.log(10.)), color='g')
# 	plt.xlabel('Level')
# 	plt.ylabel('Delta log likelihood')
# 	plt.show()
#
