#include "MyModel.h"
#include "DNest4/code/DNest4.h"

// Initial values for static variables
Mode MyModel::mode = Mode::first;
std::vector<double> MyModel::fs;
std::vector<double> MyModel::gs;

MyModel::MyModel()
{

}

void MyModel::from_prior(DNest4::RNG& rng)
{
    x.from_prior(rng);
}

double MyModel::perturb(DNest4::RNG& rng)
{
    return x.perturb(rng);
}

double MyModel::log_likelihood() const
{
    double logL = 0.0;

    if(mode == Mode::first)
        logL = x.f();
    else if(mode == Mode::second)
        logL = x.g();
    else if(mode == Mode::sum)
    {
        double f = x.f();
        double g = x.g();
        for(size_t i=0; i<fs.size(); ++i)
        {
            if(f > fs[i])
                logL += 1.0;
            else
                break;
        }
        for(size_t i=0; i<fs.size(); ++i)
        {
            if(g > gs[i])
                logL += 1.0;
            else
                break;
        }
    }
    else
        std::cerr << "Unknown mode." << std::endl;

    return logL;
}

void MyModel::print(std::ostream& out) const
{
    x.print(out);

    // Append f and g
    out << ' ' << x.f() << ' ' << x.g() << ' ';
}

std::string MyModel::description() const
{
    return std::string("");
}

void MyModel::set_mode(const Mode& new_mode)
{
    mode = new_mode;
}

void MyModel::load_data()
{
    fs.clear();
    gs.clear();
    std::fstream fin;

    double a, b, c, d;

    fin.open("sample_info_first.txt", std::ios::in);
    fin.ignore(1000000, '\n'); // Skip header line
    while(fin >> a && fin >> b && fin >> c && fin >> d)
        fs.push_back(b);
    fin.close();

    fin.open("sample_info_second.txt", std::ios::in);
    fin.ignore(1000000, '\n'); // Skip header line
    while(fin >> a && fin >> b && fin >> c && fin >> d)
        gs.push_back(b);
    fin.close();

    std::sort(fs.begin(), fs.end());
    std::sort(gs.begin(), gs.end());

}

