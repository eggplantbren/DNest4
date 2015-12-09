#ifndef _Level_
#define _Level_

namespace DNest4
{

/*
* An object of this class represents a level, or a likelihood
* threshold.
*/
class Level
{
	private:
		// The loglikelihood and tiebreaker of the level
		double log_likelihood;
		double tiebreaker;

		// Counts for compression refinement
		unsigned long long int visits, exceeds;

		// Counts for acceptance rate
		unsigned long long int accepts, tries;

	public:
		// A do-nothing constructor
		Level();

		// Specify log_likelihood and tiebreaker
		Level(double log_likelihood, double tiebreaker);
};

} // namespace DNest4

#include "LevelImpl.h"

#endif

