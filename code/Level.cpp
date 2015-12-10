#include "Level.h"

namespace DNest4
{

Level::Level()
:log_likelihood(LikelihoodType())
,log_X(0.)
,visits(0)
,exceeds(0)
,accepts(0)
,tries(0)
{

}

Level::Level(const LikelihoodType& log_likelihood)
:log_likelihood(log_likelihood)
,log_X(0.)
,visits(0)
,exceeds(0)
,accepts(0)
,tries(0)
{

}

} // namespace DNest4

