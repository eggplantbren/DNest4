#ifndef DNest4_BasicCircular
#define DNest4_BasicCircular

#include "ConditionalPrior.h"
#include "../../RNG.h"
#include "../../Utils.h"

namespace DNest4
{

class BasicCircular:public ConditionalPrior
{
	private:
		// Limits
		double x_min, x_max, y_min, y_max, size;
		double mu_min, mu_max;

		// Center and width of circle
		double xc, yc;
		double width;

		// Mean of exponential ConditionalPrior for masses
		double mu;

		double perturb_hyperparameters(DNest4::RNG& rng);

	public:
		BasicCircular(double x_min, double x_max,
					double y_min, double y_max,
					double mu_min, double mu_max);

		void from_prior(DNest4::RNG& rng);

		double log_pdf(const std::vector<double>& vec) const;
		void from_uniform(std::vector<double>& vec) const;
		void to_uniform(std::vector<double>& vec) const;

		void print(std::ostream& out) const;
};

} // namespace DNest4

#endif

