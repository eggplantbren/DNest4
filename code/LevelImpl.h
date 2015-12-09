namespace DNest4
{

Level::Level()
:visits(0)
,exceeds(0)
,accepts(0)
,tries(0)
{

}

Level::Level(double log_likelihood, double tiebreaker)
:log_likelihood(log_likelihood)
,tiebreaker(tiebreaker)
,visits(0)
,exceeds(0)
,accepts(0)
,tries(0)
{

}

} // namespace DNest4

