#include "LikelihoodType.h"
#include "Utils.h"
#include <limits>
#include <cassert>

namespace DNest4
{

LikelihoodType::LikelihoodType()
:value(std::numeric_limits<double>::lowest())
,tiebreaker(0.)
{

}

LikelihoodType::LikelihoodType(double value, double tiebreaker)
:value(value)
,tiebreaker(tiebreaker)
{
	assert(tiebreaker > 0. && tiebreaker < 1.);
}

double LikelihoodType::perturb(RNG& rng)
{
	tiebreaker += rng.randh();
	wrap(tiebreaker, 0., 1.);
	return 0.;
}

// Less-than operator
bool LikelihoodType::operator < (const LikelihoodType& other) const
{
	if(value < other.value)
		return true;
	if((value == other.value) &&
		(tiebreaker < other.tiebreaker))
		return true;
	return false;
}

} // namespace DNest4

