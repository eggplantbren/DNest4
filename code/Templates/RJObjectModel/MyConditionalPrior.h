#ifndef DNest4_Template_MyConditionalPrior
#define DNest4_Template_MyConditionalPrior

#include "DNest4/code/DNest4.h"

class MyConditionalPrior:public DNest4::ConditionalPrior
{
    private:

        double perturb_hyperparameters(DNest4::RNG& rng);

    public:
        MyConditionalPrior();

        void from_prior(DNest4::RNG& rng);

        double log_pdf(const std::vector<double>& vec) const;
        void from_uniform(std::vector<double>& vec) const;
        void to_uniform(std::vector<double>& vec) const;

        void print(std::ostream& out) const;
        static const int weight_parameter = 1;
};

#endif

