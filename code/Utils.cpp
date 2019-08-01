#include "Utils.h"

namespace DNest4
{

double mod(double y, double x)
{
    assert(x > 0.);
    return (y/x - floor(y/x))*x;
}

void wrap(double& x, double min, double max)
{
    x = DNest4::mod(x - min, max - min) + min;
}

int mod(int y, int x)
{
    assert(x > 0);
    if(y >= 0)
        return y - (y/x)*x;
    else
        return (x-1) - DNest4::mod(-y-1, x);
}

double logsumexp(double* logv, int n)
{
    assert(n > 1);
    double max = logv[0];
    for(int i=1; i<n; i++)
        if(logv[i] > max)
            max = logv[i];
    double answer = 0;
    // log(sum(exp(logf))   = log(sum(exp(logf - max(logf) + max(logf)))
    //          = max(logf) + log(sum(exp(logf - max(logf)))
    for(int i=0; i<n; i++)
        answer += exp(logv[i] - max);
    answer = max + log(answer);
    return answer;
}

double logsumexp(const std::vector<double>& logv)
{
    int n = static_cast<int>(logv.size());
    //if(n<=1)
    //  cout<<"Warning in logsumexp"<<endl;
    double max = *max_element(logv.begin(), logv.end());
    double answer = 0;
    // log(sum(exp(logf))   = log(sum(exp(logf - max(logf) + max(logf)))
    //          = max(logf) + log(sum(exp(logf - max(logf)))
    for(int i=0; i<n; i++)
        answer += exp(logv[i] - max);
    answer = max + log(answer);
    return answer;
}

double logsumexp(double a, double b)
{
    double x[2] = {a,b};
    return DNest4::logsumexp(x, 2);
}

double logdiffexp(double a, double b)
{
    assert(a > b);
    double biggest = a;
    a -= biggest;
    b -= biggest;
    return log(exp(a) - exp(b)) + biggest;
}

double normal_cdf(double x)
{
    return 0.5*(1. + erf(x/sqrt(2.)));
}

// Compute the inverse of the Normal Cumulative Density Function
// Original FORTRAN77 version by Michael Wichura.
// C++ version by John Burkardt (GNU LGPL license) available at 
// http://people.sc.fsu.edu/~jburkardt/cpp_src/asa241/asa241.html
// Reference:
//    Michael Wichura
//    The Percentage Points of the Normal Distribution
//    Algorithm AS 241, Applied Statistics, 37, 3, 477-484, 1988
double normal_inverse_cdf(double p)
{
    static double a[8] = { 
        3.3871328727963666080,     1.3314166789178437745e+2,
        1.9715909503065514427e+3,  1.3731693765509461125e+4,
        4.5921953931549871457e+4,  6.7265770927008700853e+4,
        3.3430575583588128105e+4,  2.5090809287301226727e+3 };
    static double b[8] = {
        1.0,                       4.2313330701600911252e+1,
        6.8718700749205790830e+2,  5.3941960214247511077e+3,
        2.1213794301586595867e+4,  3.9307895800092710610e+4,
        2.8729085735721942674e+4,  5.2264952788528545610e+3 };
    static double c[8] = {
        1.42343711074968357734,     4.63033784615654529590,
        5.76949722146069140550,     3.64784832476320460504,
        1.27045825245236838258,     2.41780725177450611770e-1,
        2.27238449892691845833e-2,  7.74545014278341407640e-4 };
    static double const1 = 0.180625;
    static double const2 = 1.6;
    static double d[8] = {
        1.0,                        2.05319162663775882187,
        1.67638483018380384940,     6.89767334985100004550e-1,
        1.48103976427480074590e-1,  1.51986665636164571966e-2,
        5.47593808499534494600e-4,  1.05075007164441684324e-9 };
    static double e[8] = {
        6.65790464350110377720,     5.46378491116411436990,
        1.78482653991729133580,     2.96560571828504891230e-1,
        2.65321895265761230930e-2,  1.24266094738807843860e-3,
        2.71155556874348757815e-5,  2.01033439929228813265e-7 };
    static double f[8] = {
        1.0,                        5.99832206555887937690e-1,
        1.36929880922735805310e-1,  1.48753612908506148525e-2,
        7.86869131145613259100e-4,  1.84631831751005468180e-5,
        1.42151175831644588870e-7,  2.04426310338993978564e-15 };
    double q;
    double r;
    static double split1 = 0.425;
    static double split2 = 5.0;
    double value;

    if(p < 0.0 || p > 1.0)
        throw std::domain_error("Input to normal_inverse_cdf must be in (0, 1).");

    q = p - 0.5;

    if(fabs(q) <= split1)
    {
        r = const1 - q * q;
        value = q * r8poly_value(8, a, r) / r8poly_value(8, b, r);
    }
    else
    {
        if(q < 0.0)
            r = p;
        else
            r = 1.0 - p;

        if(r <= 0.0)
            return 0.; // IFAULT=1

        r = sqrt(-log(r));

        if(r <= split2)
        {
            r = r - const2;
            value = r8poly_value(8, c, r) / r8poly_value(8, d, r); 
        }
        else
        {
            r = r - split2;
            value = r8poly_value(8, e, r) / r8poly_value(8, f, r);
        }

        if(q < 0.0)
            value = -value;
    }

    return value;
}

// Evaluates a double precision polynomial. Author: John Burkardt
// http://people.sc.fsu.edu/~jburkardt/cpp_src/asa241/asa241.html
double r8poly_value(int n, double a[], double x)
{
    int i;
    double value = 0.0;
    for (i=n-1; 0<=i; i--)
        value = value * x + a[i];
    return value;
}

double perturb_ns(std::vector<double>& ns, RNG& rng)
{
    double logH = 0.0;

    if(rng.rand() <= 0.5)
    {
        // Perturb a single n
        int k = rng.rand_int(ns.size());
        logH -= -0.5*pow(ns[k], 2);
        ns[k] += rng.randh();
        logH += -0.5*pow(ns[k], 2);
    }
    else
    {
        // Resample several ns
        int reps = static_cast<int>(pow(ns.size(), rng.rand()));
        for(int i=0; i<reps; ++i)
        {
            int k = rng.rand_int(ns.size());
            ns[k] = rng.randn();
        }
    }
    return logH;
}

} // namespace DNest4

