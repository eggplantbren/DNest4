#ifndef DNest4_MyConditionalPrior
#define DNest4_MyConditionalPrior

#include "DNest4/code/DNest4.h"

// Based on ClassicMassInf1D from RJObject
// Think of "position x" as log-period
// and mass as amplitude
class MyConditionalPrior:public DNest4::ConditionalPrior
{
	private:
		// Limits
		double x_min, x_max;
		double mu_min, mu_max;

		// Mean of exponential ConditionalPrior for amplitudes
		double mu;

		double perturb_hyperparameters(DNest4::RNG& rng);

	public:
		MyConditionalPrior(double x_min, double x_max,
					double mu_min, double mu_max);

		void from_prior(DNest4::RNG& rng);

		double log_pdf(const std::vector<double>& vec) const;
		void from_uniform(std::vector<double>& vec) const;
		void to_uniform(std::vector<double>& vec) const;

		void print(std::ostream& out) const;
		static const int weight_parameter = 1;

};

#endif

