template<class ConditionalPrior>
RJObject<ConditionalPrior>::RJObject(int num_dimensions, int max_num_components, bool fixed,
				const ConditionalPrior& conditional_prior,
                PriorType prior_type)
:prior_type(prior_type)
,num_dimensions(num_dimensions)
,max_num_components(max_num_components)
,fixed(fixed)
,conditional_prior(conditional_prior)
,num_components(0)
{

}

template<class ConditionalPrior>
void RJObject<ConditionalPrior>::from_prior(RNG& rng)
{
	added.resize(0);
	removed.resize(0);

	// Generate the hyperparameters from their prior
	conditional_prior.from_prior(rng);

	int num = max_num_components;

	// Generate from {0, 1, 2, ..., max_num_components}
	if(!fixed)
    {
        if(prior_type == PriorType::uniform)
        {
		    num = rng.rand_int(max_num_components + 1);
        }
        else if(prior_type == PriorType::log_uniform)
        {
            do
            {
                num = rng.rand_int(max_num_components + 1);
            }while(rng.rand() >= 1.0/(num + 1));
        }
    }

	// Resize the vectors of component properties
	components.resize(0);
	u_components.resize(0);

	// Generate components
	num_components = 0;
	for(int i=0; i<num; i++)
		add_component(rng);

    components_changed_flag = true;
}

template<class ConditionalPrior>
double RJObject<ConditionalPrior>::perturb_components(RNG& rng)
{
	if(num_components == 0)
		return 0.;

    double chance = pow(10.0, -3*std::abs(rng.randt2()));

	// A flag for whether each component gets changed or not
	std::vector<bool> change(num_components, false);
	int count = 0;
	for(int i=0; i<num_components; i++)
	{
		if(rng.rand() <= chance)
		{
			change[i] = true;
			count++;
		}
	}
	// At least do one...
	if(count == 0)
		change[rng.rand_int(num_components)] = true;

	for(int i=0; i<num_components; i++)
	{
		if(change[i])
		{
			// Accumulate added/removed component info
			removed.push_back(components[i]);

			// Perturb
			for(int j=0; j<num_dimensions; j++)
			{
				u_components[i][j] += rng.randh();
				u_components[i][j] = DNest4::mod(
							u_components[i][j], 1.0);
				components[i][j] = u_components[i][j];
			}
			conditional_prior.from_uniform(components[i]);

			// Accumulate added/removed component info
			added.push_back(components[i]);
		}
	}

	return 0.;
}

template<class ConditionalPrior>
double RJObject<ConditionalPrior>::add_component(RNG& rng)
{
	if(num_components >= max_num_components)
	{
		std::cerr<<"# WARNING: Trying to add component ";
		std::cerr<<"but already full!"<<std::endl;
		return 0.;
	}

	// Increment counter
	num_components++;

	// Generate component
	std::vector<double> component(num_dimensions);
	for(int j=0; j<num_dimensions; j++)
		component[j] = rng.rand();
	u_components.push_back(component);
	conditional_prior.from_uniform(component);
	components.push_back(component);

	// Accumulate added/removed component info
	added.push_back(component);

	return 0.;
}

template<class ConditionalPrior>
double RJObject<ConditionalPrior>::perturb_num_components(RNG& rng)
{
	double logH = 0.;

	// Work out how many components we will have after the change
	double delta = max_num_components*rng.randh();
	int difference = (int)delta;
	// In case difference is zero, make it +1 or -1. Also, just do this
    // sometimes just because it's gentle
	if(difference == 0)
	{
		if(rng.rand() <= 0.5)
			difference = -1;
		else
			difference =  1;
	}
	int new_num_components = DNest4::mod(num_components + difference, max_num_components + 1);
	difference = new_num_components - num_components;

    // Add to logH for non-uniform prior on N
	if(prior_type == PriorType::log_uniform)
		logH += log(num_components + 1) - log(new_num_components + 1);

	// Now do the required changes
	if(difference > 0)
	{
		for(int i=0; i<difference; i++)
			logH += add_component(rng);
	}
	else
	{
		for(int i=0; i<-difference; i++)
			logH += remove_component(rng);
	}

	return logH;
}

template<class ConditionalPrior>
double RJObject<ConditionalPrior>::perturb(RNG& rng, bool hyperparameters)
{
	if(max_num_components == 0)
		return 0.;

	added.resize(0);
	removed.resize(0);
    components_changed_flag = true;

	double logH = 0.;

	int which;
    do
    {
        which = (fixed)?(1 + rng.rand_int(2)):(rng.rand_int(3));
    }while(!hyperparameters && which == 1);

	if(which == 0)
	{
		// Do some birth or death
		logH += perturb_num_components(rng);
	}
	else if(which == 1)
	{
		// Change the hyperparameters
		if(rng.rand() <= 0.5)
		{
			logH += conditional_prior.perturb1(rng, components, u_components);
            components_changed_flag = false;
		}
		else
		{
			removed = components;
			logH += conditional_prior.perturb2(rng, components, u_components);
			added = components;
		}
	}
	else if(which == 2)
	{
		logH += perturb_components(rng);
	}

	return logH;
}

template<class ConditionalPrior>
double RJObject<ConditionalPrior>::remove_component(RNG& rng)
{
	if(num_components <= 0)
	{
		std::cerr<<"# WARNING: Trying to remove component ";
		std::cerr<<"but already empty!"<<std::endl;
		return 0.;
	}

	// Find one to delete
	int i = rng.rand_int(num_components);

	// Accumulate added/removed component info
	removed.push_back(components[i]);

	// Delete it
	u_components.erase(u_components.begin() + i);
	components.erase(components.begin() + i);

	// Decrement counter
	num_components--;

	return 0.;
}

template<class ConditionalPrior>
void RJObject<ConditionalPrior>::print(std::ostream& out) const
{
	out<<num_dimensions<<' '<<max_num_components<<' ';
	conditional_prior.print(out); out<<' ';
	out<<num_components<<' ';

	// Write out components
	for(int j=0; j<num_dimensions; j++)
	{
		for(int i=0; i<num_components; i++)
			out<<components[i][j]<<' ';

		// Pad with zeros (turned-off components)
		for(int i=num_components; i<max_num_components; i++)
			out<<0.<<' ';
	}
}

template<class ConditionalPrior>
void RJObject<ConditionalPrior>::read(std::istream& in)
{
    double a, b;

    in>>a>>b;
    num_dimensions = (int)a;
    max_num_components = (int)b;

    conditional_prior.read(in);
    in>>a;
    num_components = (int)a;

    // Resize and read in components
    components.assign(num_components, std::vector<double>(num_dimensions, 0.0));
    for(int j=0; j<num_dimensions; ++j)
    {
        for(int i=0; i<num_components; ++i)
            in>>components[i][j];

        // Read in the zero padding
        double junk;
		for(int i=num_components; i<max_num_components; i++)
            in>>junk;
    }

    // Calculate u_components
    u_components = components;
    for(int i=0; i<num_components; ++i)
        conditional_prior.to_uniform(u_components[i]);

    // All components have been "added"
    added = components;
    removed.clear();
}

template<class ConditionalPrior>
void RJObject<ConditionalPrior>::consolidate_diff()
{
	if(ConditionalPrior::weight_parameter == -1)
	{
		std::cerr<<"# WARNING: consolidate_diff() failed."<<std::endl;
		return;
	}

	for(size_t i=0; i<removed.size(); i++)
	{
		removed[i][ConditionalPrior::weight_parameter] *= -1;
		added.push_back(removed[i]);
	}
	removed.clear();
}

template<class ConditionalPrior>
void RJObject<ConditionalPrior>::clear()
{
    num_components = 0;
    removed = components;
    components.clear();
    u_components.clear();
    added.clear();
}

template<class ConditionalPrior>
void RJObject<ConditionalPrior>::set_components
            (const std::vector<std::vector<double>>& comp)
{
    removed = components;
    components = comp;
    u_components = comp;
    for(size_t i=0; i<u_components.size(); ++i)
        conditional_prior.to_uniform(u_components[i]);
    added = components;
    num_components = components.size();
}




