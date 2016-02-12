#ifndef DNest4_GalaxyField_MyConditionalPrior
#define DNest4_GalaxyField_MyConditionalPrior

#include "DNest4/code/DNest4.h"

// Hyperparameters setting interim prior for galaxy properties
class MyConditionalPrior:public DNest4::ConditionalPrior
{
	private:
		// Limits
		double x_min, x_max, y_min, y_max;
		double fluxlim_min, fluxlim_max;
		double radiuslim_min, radiuslim_max;

		// Lower limit and 1/slope for Pareto interim prior
		// for masses
		double fluxlim;
		double gamma;

		// Lower limit and 1/slope for Pareto interim prior
		// for radii
		double radiuslim;
		double gamma_radius;

		// Uniform interim prior for radius ratio and mass ratio
		double a1, b1;
		double a2, b2;

		double perturb_hyperparameters(DNest4::RNG& rng);

	public:
		MyConditionalPrior(double x_min, double x_max,
					double y_min, double y_max,
					double fluxlim_min, double fluxlim_max);

		void from_prior(DNest4::RNG& rng);

		double log_pdf(const std::vector<double>& vec) const;
		void from_uniform(std::vector<double>& vec) const;
		void to_uniform(std::vector<double>& vec) const;

		void print(std::ostream& out) const;
};

#endif

