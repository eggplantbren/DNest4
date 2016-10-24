#ifndef DNest4_MyConditionalPrior
#define DNest4_MyConditionalPrior

#include "DNest4/code/DNest4.h"

// Based on ClassicMassInf1D from RJObject
// Think of "position x" as log-period
// and mass as amplitude
class MyConditionalPrior:public DNest4::ConditionalPrior
{
	private:
        static const DNest4::Cauchy cauchy;

		double location_log_period, scale_log_period;
        double location_log_amplitude, scale_log_amplitude;

		double perturb_hyperparameters(DNest4::RNG& rng);

	public:
		MyConditionalPrior();

		void from_prior(DNest4::RNG& rng);

		double log_pdf(const std::vector<double>& vec) const;
		void from_uniform(std::vector<double>& vec) const;
		void to_uniform(std::vector<double>& vec) const;

		void print(std::ostream& out) const;
        void read(std::istream& in);
		static const int weight_parameter = 1;

};

#endif

