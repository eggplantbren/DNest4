#include "Options.h"
#include <fstream>
#include <iostream>
#include <cassert>
#include <cstdlib>

namespace DNest4
{

Options::Options(unsigned int num_particles,
		 unsigned int new_level_interval,
		 unsigned int save_interval,
		 unsigned int thread_steps,
		 unsigned int max_num_levels,
		 double lambda,
		 double beta,
		 unsigned int max_num_saves)
:num_particles(num_particles)
,new_level_interval(new_level_interval)
,save_interval(save_interval)
,thread_steps(thread_steps)
,max_num_levels(max_num_levels)
,lambda(lambda)
,beta(beta)
,max_num_saves(max_num_saves)
,sample_file("sample.txt")
,sample_info_file("sample_info.txt")
,levels_file("levels.txt")
{
	assert(num_particles > 0 && new_level_interval > 0 &&
		max_num_levels > 0 && lambda > 0. &&
		beta >= 0.);
}

Options::Options(const char* filename)
:sample_file("sample.txt")
,sample_info_file("sample_info.txt")
,levels_file("levels.txt")
{
	load(filename);
}

void Options::load(const char* filename)
{
	std::fstream fin(filename, std::ios::in);
	if(!fin)
	{
		std::cerr<<"ERROR: Cannot open file "<<filename<<"."<<std::endl;
		exit(0);
	}

	// Read past comment lines at the top of the file
	while(fin.peek() == '#')
		fin.ignore(1000000, '\n');

	fin>>num_particles;	fin.ignore(1000000, '\n');
	fin>>new_level_interval;	fin.ignore(1000000, '\n');
	fin>>save_interval;	fin.ignore(1000000, '\n');
	fin>>thread_steps;	fin.ignore(1000000, '\n');
	fin>>max_num_levels;	fin.ignore(1000000, '\n');
	fin>>lambda;		fin.ignore(1000000, '\n');
	fin>>beta;		fin.ignore(1000000, '\n');
	fin>>max_num_saves;		fin.ignore(1000000, '\n');
	fin>>sample_file;		fin.ignore(1000000, '\n');
	fin>>sample_info_file;	fin.ignore(1000000, '\n');
	fin>>levels_file;

	if (sample_file.compare("#") == 0) sample_file = "sample.txt";
	if (sample_info_file.compare("#") == 0) sample_info_file = "sample_info.txt";
	if (levels_file.compare("#") == 0) levels_file = "levels.txt";

	fin.close();

	assert(num_particles > 0 && new_level_interval > 0 &&
			thread_steps > 0 && max_num_levels > 0 && lambda > 0. &&
			beta >= 0.);
}

void Options::print(std::ostream& out) const
{
	out<<num_particles<<' ';
	out<<new_level_interval<<' ';
	out<<save_interval<<' ';
	out<<thread_steps<<' ';
	out<<max_num_levels<<' ';
	out<<lambda<<' ';
	out<<beta<<' ';
	out<<max_num_saves<<' ';

	out<<sample_file<<' ';
	out<<sample_info_file<<' ';
	out<<levels_file<<' ';
}

void Options::read(std::istream& in)
{
	in>>num_particles;
	in>>new_level_interval;
	in>>save_interval;
	in>>thread_steps;
	in>>max_num_levels;
	in>>lambda;
	in>>beta;
	in>>max_num_saves;

	in>>sample_file;
	in>>sample_info_file;
	in>>levels_file;
}

} // namespace DNest4

// Operator << which just calls print
using namespace std;
ostream& operator << (ostream& out, const DNest4::Options& o)
{
	o.print(out);
	return out;
}

istream& operator >> (istream& in, DNest4::Options& o)
{
	o.read(in);
	return in;
}

