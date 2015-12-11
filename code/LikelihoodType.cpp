#include "LikelihoodType.h"
#include "Utils.h"
#include <limits>
#include <cassert>

namespace DNest4
{

LikelihoodType::LikelihoodType()
:log_likelihood(std::numeric_limits<double>::lowest())
,tiebreaker(0.)
{

}

LikelihoodType::LikelihoodType(double log_likelihood, double tiebreaker)
:log_likelihood(log_likelihood)
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
	if(log_likelihood < other.log_likelihood)
		return true;
	if((log_likelihood == other.log_likelihood) &&
		(tiebreaker < other.tiebreaker))
		return true;
	return false;
}

} // namespace DNest4

