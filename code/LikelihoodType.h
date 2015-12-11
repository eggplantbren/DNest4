#ifndef DNest4_LikelihoodType
#define DNest4_LikelihoodType

#include "RNG.h"

namespace DNest4
{

/*
* An object of this class represents a log likelihood value
* and a tiebreaker value \in [0, 1].
*/
class LikelihoodType
{
	private:
		double value;
		double tiebreaker;

	public:
		// Sets value to "-Infinity"
		// and tiebreaker to zero
		LikelihoodType();
		LikelihoodType(double value, double tiebreaker);

		// Perturb the tiebreaker
		double perturb(RNG& rng);

		// Getters
		double get_value() const
		{ return value; }
		double get_tiebreaker() const
		{ return tiebreaker; }

		// Less-than operator
		bool operator < (const LikelihoodType& other) const;
}; // class LikelihoodType

} // namespace DNest4

#endif

