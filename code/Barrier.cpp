#include "Barrier.h"

using namespace DNest4;

Barrier::Barrier(unsigned int count)
:threshold(count)
,count(count)
,generation(0)
{

}

void Barrier::wait()
{
	unsigned int gen = generation;
	std::unique_lock<std::mutex> lock{the_mutex};
	if((--count) == 0)
	{
		generation++;
		count = threshold;
		cond.notify_all();
	}
	else
		cond.wait(lock, [this, gen] { return gen != generation; });
}

