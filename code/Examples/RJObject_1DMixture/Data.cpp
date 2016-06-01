#include "Data.h"
#include <fstream>
#include <iostream>

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
		cerr<<"# Error. Couldn't open file "<<filename<<"."<<endl;
		return;
	}

	// Empty the vector
	x.clear();

	double temp1;
	while(fin>>temp1)
		x.push_back(temp1);

	cout<<"# Loaded "<<x.size()<<" data points from file "
			<<filename<<"."<<endl;
	fin.close();
}

