#include "Level.h"
#include <cassert>
#include <cmath>

using namespace std;

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

void Level::recalculate_log_X(vector<Level>& levels, double compression,
												unsigned int regularisation)
{
	assert(levels.size() > 0);

	levels[0].log_X = 0.0;
	for(size_t i=1; i<levels.size(); ++i)
		levels[i].log_X = levels[i-1].log_X + log((double)(levels[i-1].exceeds + (1./compression)*regularisation)/(double)(levels[i-1].visits + regularisation));
}

void Level::renormalise_visits(vector<Level>& levels,
									unsigned int regularisation)
{
	for(auto& level: levels)
	{
		if(level.tries >= regularisation)
		{
			level.accepts = ((double)(level.accepts+1)/(double)(level.tries+1))*regularisation;
			level.tries = regularisation;
		}
		if(level.visits >= regularisation)
		{
			level.exceeds = ((double)(level.exceeds+1)/(double)(level.visits+1))*regularisation;
			level.visits = regularisation;
		}
	}
}

void Level::print(ostream& out) const
{
	log_likelihood.print(out);
	out<<log_X<<' ';
	out<<visits<<' '<<exceeds<<' '<<accepts<<' '<<tries<<' ';
}

void Level::read(istream& in)
{
	log_likelihood.read(in);
	in>>log_X;
	in>>visits;
	in>>exceeds;
	in>>accepts;
	in>>tries;
}

} // namespace DNest4

