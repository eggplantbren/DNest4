#ifndef _LikelihoodType_
#define _LikelihoodType_

namespace DNest4
{

/*
* An object of this class represents a log likelihood value
* and a tiebreaker value \in [0, 1].
*/
class LikelihoodType
{
	private:
		double log_likelihood;
		double tiebreaker;

	public:
		// Sets log_likelihood to "-Infinity"
		// and tiebreaker to zero
		LikelihoodType();
		LikelihoodType(double log_likelihood, double tiebreaker);

		// Less-than operator
		bool operator < (const LikelihoodType& other) const;
}; // class LikelihoodType

} // namespace DNest4

#include "LikelihoodTypeImpl.h"
#endif

