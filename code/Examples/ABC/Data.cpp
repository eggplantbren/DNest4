#include "Data.h"
#include <iostream>
#include <fstream>
#include <algorithm>

using namespace std;

Data Data::instance;

Data::Data()
{

}

void Data::load(const char* filename)
{
	fstream fin(filename, ios::in);
	if(!fin)
	{
		cerr<<"# ERROR: Couldn't open file "<<filename<<"."<<endl;
		return;
	}

	double temp;
	x.clear();
	while(fin>>temp)
		x.push_back(temp);
	cout<<"# Loaded "<<x.size()<<" points from file "<<filename<<"."<<endl;

	fin.close();

	x_min = *min_element(x.begin(), x.end());
	x_max = *max_element(x.begin(), x.end());
}

