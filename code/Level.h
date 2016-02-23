#ifndef DNest4_Level
#define DNest4_Level

#include "LikelihoodType.h"
#include <vector>
#include <ostream>
#include <istream>

namespace DNest4
{

/*
* An object of this class represents a level, or a likelihood
* threshold.
*/
class Level
{
	private:
		// The loglikelihood of the level
		LikelihoodType log_likelihood;

		// Estimated compression of the level
		double log_X;

		// unsigned long long int is a well-defined thing in C++11 !!!
		// Counts for compression refinement
		unsigned long long int visits, exceeds;

		// Counts for acceptance rate
		unsigned long long int accepts, tries;

	public:
		// A do-nothing constructor
		Level();

		// Specify log_likelihood
		Level(const LikelihoodType& log_likelihood);

		// Getter for the log_likelihood
		const LikelihoodType& get_log_likelihood() const
		{ return log_likelihood; }

		// Getters
		unsigned int get_visits() const
		{ return visits; }
		unsigned int get_exceeds() const
		{ return exceeds; }
		unsigned int get_accepts() const
		{ return accepts; }
		unsigned int get_tries() const
		{ return tries; }
		double get_log_X() const
		{ return log_X; }

		// Incrementors
		void increment_visits(int diff) { visits += diff; }
		void increment_exceeds(int diff) { exceeds += diff; }
		void increment_accepts(int diff) { accepts += diff; }
		void increment_tries(int diff) { tries += diff; }

		// Print and read
		void print(std::ostream& out) const;
		void read(std::istream& in);

		// Operations on sets of levels
		static void recalculate_log_X(std::vector<Level>& levels,
										double compression,
										unsigned int regularisation);
		static void renormalise_visits(std::vector<Level>& levels,
										unsigned int regularisation);
};

} // namespace DNest4

#endif

