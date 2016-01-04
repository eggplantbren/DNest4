#include "Data.h"
#include <fstream>
#include <vector>

using namespace std;

// The static instance
Data Data::instance;

Data::Data()
{

}

void Data::load(const char* filename)
{
	// Vectors to hold the data
	std::vector<double> _x;
	std::vector<double> _y;

	// Open the file
	fstream fin(filename, ios::in);

	// Temporary variables
	double temp1, temp2;

	// Read until end of file
	while(fin>>temp1 && fin>>temp2)
	{
		_x.push_back(temp1);
		_y.push_back(temp2);
	}

	// Close the file
	fin.close();

	// Copy the data to the valarrays
	x = valarray<double>(&_x[0], _x.size());
	y = valarray<double>(&_y[0], _y.size());
}

