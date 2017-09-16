#ifndef DNest4_GalaxyField_MyConditionalPrior
#define DNest4_GalaxyField_MyConditionalPrior

#include "DNest4/code/DNest4.h"

// Hyperparameters setting interim prior for galaxy properties
class MyConditionalPrior:public DNest4::ConditionalPrior
{
	private:
		// Limits
		double x_min, x_max, y_min, y_max, scale;

		double typical_flux, dev_log_flux;
		double typical_radius, dev_log_radius;

		double perturb_hyperparameters(DNest4::RNG& rng);

	public:
		MyConditionalPrior(double x_min, double x_max,
					       double y_min, double y_max);

		void from_prior(DNest4::RNG& rng);

		double log_pdf(const std::vector<double>& vec) const;
		void from_uniform(std::vector<double>& vec) const;
		void to_uniform(std::vector<double>& vec) const;

		void print(std::ostream& out) const;
};

#endif

