#ifndef DNest4_Barrier
#define DNest4_Barrier

#include <mutex>
#include <condition_variable>

namespace DNest4
{

/*
* Barrier class based on an answer at
* http://stackoverflow.com/questions/24465533/implementing-boostbarrier-in-c11
*/

class Barrier
{
	private:
		std::mutex the_mutex;
		std::condition_variable cond;
		unsigned int threshold;
		unsigned int count;
		unsigned int generation;

	public:
		// Constructor: initialise count
		explicit Barrier(unsigned int count);

		// Wait method
		void wait();
};

} // namespace DNest4

#endif

