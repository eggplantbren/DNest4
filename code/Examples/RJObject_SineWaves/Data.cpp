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

	// Empty the vectors
	t.clear();
	y.clear();

	double temp1, temp2, temp3;
	while(fin>>temp1 && fin>>temp2 && fin>>temp3)
	{
		t.push_back(temp1);
		y.push_back(temp2);
        sig.push_back(temp3);
	}
	cout<<"# Loaded "<<t.size()<<" data points from file "
			<<filename<<"."<<endl;
	fin.close();
}

