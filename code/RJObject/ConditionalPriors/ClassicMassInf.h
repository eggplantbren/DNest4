#ifndef DNest4_ClassicMassInf
#define DNest4_ClassicMassInf

#include "ConditionalPrior.h"
#include "../../RNG.h"

namespace DNest4
{

class ClassicMassInf:public ConditionalPrior
{
	private:
		// Limits
		double x_min, x_max, y_min, y_max;
		double mu_min, mu_max;

		// Mean of exponential ConditionalPrior for masses
		double mu;

		double perturb_hyperparameters(RNG& rng);

	public:
		ClassicMassInf(double x_min, double x_max,
					double y_min, double y_max,
					double mu_min, double mu_max);

		void from_prior(RNG& rng);

		double log_pdf(const std::vector<double>& vec) const;
		void from_uniform(std::vector<double>& vec) const;
		void to_uniform(std::vector<double>& vec) const;

		void print(std::ostream& out) const;
};

} // namespace DNest4

#endif

