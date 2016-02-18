#ifndef DNest4_Distributions
#define DNest4_Distributions

/*
* PDFs, CDFs, and inverse CDFs for some common distributions
*/
namespace DNest4
{
	// Laplacian distribution
	double laplacian_log_pdf(double x, double location, double scale);
	double laplacian_cdf(double x, double location, double scale);
	double laplacian_cdf_inverse(double x, double location, double scale);
} // namespace DNest4

#endif

