#include "LikelihoodType.h"
#include "Utils.h"
#include <limits>
#include <cassert>
#include <iostream>

namespace DNest4
{

LikelihoodType::LikelihoodType()
:value(std::numeric_limits<double>::lowest())
,tiebreaker(0.)
{

}

LikelihoodType::LikelihoodType(double value, double tiebreaker)
{
	assert(tiebreaker > 0. && tiebreaker < 1.);
    if(std::isinf(value) || std::isnan(value))
        value = -1E300;
    this->value = value;
    this->tiebreaker = tiebreaker;
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

void LikelihoodType::print(std::ostream& out) const
{
	out<<value<<' '<<tiebreaker<<' ';
}

void LikelihoodType::read(std::istream& in)
{
	in>>value;
	in>>tiebreaker;
}

} // namespace DNest4

