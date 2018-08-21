#ifndef DNest4_RNG
#define DNest4_RNG

#include <random>

namespace DNest4
{

/*
* An object of this class represents a random number generator 
*/
class RNG
{
	private:

		// 64-bit Mersenne Twister
		std::mt19937_64 twister;

		// For uniform distribution
		std::uniform_real_distribution<double> uniform;

		// For normal distribution
		std::normal_distribution<double> normal;

    public:
        static bool randh_is_randh2;

	public:
		// Constructor
		RNG();

		// Constructor that also sets seed
		RNG(unsigned int seed);

		// Set the seed (obviously)
		void set_seed(unsigned int seed);

		// Uniform(0, 1)
		double rand();

		// Normal(0, 1)
		double randn();

        // t(location=0, scale=1, nu=2)
        double randt2();

		// My favourite heavy-tailed distribution
		double randh();

        // A more aggressive version of randh() [more mass near magnitude 1]
        double randh2();

		// Integer from {0, 1, 2, ..., N-1}
		int rand_int(int N);
}; // class RNG

} // namespace DNest4

#endif

