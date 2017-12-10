#ifndef DNest4_RJObject
#define DNest4_RJObject

#include <vector>
#include <ostream>
#include <cmath>
#include <iostream>
#include <iomanip>
#include "../Utils.h"
#include "../RNG.h"

namespace DNest4
{

// One of these determines the prior for N
enum class PriorType
{
    uniform, log_uniform
};

/*
* A class that implements basic birth-death Metropolis-Hastings
* proposals for use in DNest4.
*/

template<class ConditionalPrior>
class RJObject
{
	protected:
        // Which prior we're using for N
        PriorType prior_type;

		// How many parameters for each component
		int num_dimensions;

		// Maximum number of components allowed (minimum is zero)
		int max_num_components;

		// fixed num_components = max_num_components?
		bool fixed;

		// The hyperparameters that specify the conditional prior
		// for the components
		ConditionalPrior conditional_prior;

        // Flag to indicate whether components changed during a perturb
        bool components_changed_flag;

		// The components
		int num_components;
		std::vector< std::vector<double> > components;

		// Transformed into iid U(0, 1) priors
		std::vector< std::vector<double> > u_components;

		// Added/removed components
		std::vector< std::vector<double> > added;
		std::vector< std::vector<double> > removed;

		// Helper methods -- these do one thing at a time
		double perturb_num_components(RNG& rng);
		double perturb_components(RNG& rng);

		// Helper methods -- add or remove single component
		double add_component(RNG& rng);
		double remove_component(RNG& rng);

	public:
		/*
		* num_dimensions: number of dimensions for each object.
		* "mass" etc count. E.g. in StarField problem (x, y, flux)
		* specifies a star, so num_dimensions = 3.
		*
		* max_num_components: obvious
		* fixed: if true, doesn't do RJ steps. N will be fixed at
		* max_num_components
		*/
		RJObject(int num_dimensions, int max_num_components, bool fixed,
				    const ConditionalPrior& conditional_prior,
                    PriorType prior_type=PriorType::uniform);

		// Generate everything from the prior
		void from_prior(RNG& rng);

		// The top-level perturb method
		double perturb(RNG& rng, bool hyperparameters=true);

		// Take "removed" components and put them into "added" with
		// negative amplitude. Only applicable to certain models!
		void consolidate_diff();

		// For output
		void print(std::ostream& out) const;

        // For input (if the need arises)
        void read(std::istream& in);

		// Getter for components
		const std::vector< std::vector<double> >& get_components() const
		{ return components; }

		// Getters for the diff
		const std::vector< std::vector<double> >& get_added() const
		{ return added; }

		// Getters for the diff
		const std::vector< std::vector<double> >& get_removed() const
		{ return removed; }

		// Getter for hyperparameters
		const ConditionalPrior& get_conditional_prior() const
		{ return conditional_prior; }

        bool components_changed() const
        { return components_changed_flag; }

        int get_max_num_components() const
        { return max_num_components; }

        // In case someone wants to clear everything
        void clear();

        // A setter for the components
        void set_components(const std::vector<std::vector<double>>& comp);
};

#include "RJObjectImpl.h"

} // namespace DNest4

#endif

