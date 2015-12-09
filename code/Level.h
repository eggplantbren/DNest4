#ifndef _Level_
#define _Level_

#include "LikelihoodType.h"

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
};

} // namespace DNest4

#include "LevelImpl.h"

#endif

