#ifndef DNest4_Template_MyConditionalPrior
#define DNest4_Template_MyConditionalPrior

#include "DNest4/code/DNest4.h"

/*
* Represents the conditional prior for mixture components
* in (mu, log_sigma) space
*/

class MyConditionalPrior:public DNest4::ConditionalPrior
{
	private:
        // Location and scale parameter for mu prior
        double location_mu;
        double scale_mu;

        // Location and scale parameter for log sigma
        double location_log_sigma;
        double scale_log_sigma;

        // Scale parameter for log weight
        double scale_log_weight;

		double perturb_hyperparameters(DNest4::RNG& rng);

	public:
		MyConditionalPrior();

		void from_prior(DNest4::RNG& rng);

		double log_pdf(const std::vector<double>& vec) const;
		void from_uniform(std::vector<double>& vec) const;
		void to_uniform(std::vector<double>& vec) const;

		void print(std::ostream& out) const;
		static const int weight_parameter = 1;
};

#endif

