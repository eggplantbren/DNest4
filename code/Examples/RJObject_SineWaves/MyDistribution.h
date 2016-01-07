#ifndef _MyDistribution_
#define _MyDistribution_

#include <Distributions/Distribution.h>

// Based on ClassicMassInf1D from RJObject
// Think of "position x" as log-period
// and mass as amplitude
class MyDistribution:public Distribution
{
	private:
		// Limits
		double x_min, x_max;
		double mu_min, mu_max;

		// Mean of exponential distribution for amplitudes
		double mu;

		double perturb_parameters();

	public:
		MyDistribution(double x_min, double x_max,
					double mu_min, double mu_max);

		void fromPrior();

		double log_pdf(const std::vector<double>& vec) const;
		void from_uniform(std::vector<double>& vec) const;
		void to_uniform(std::vector<double>& vec) const;

		void print(std::ostream& out) const;
		static const int weight_parameter = 1;

};

#endif

