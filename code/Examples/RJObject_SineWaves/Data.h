#ifndef DNest4_Data
#define DNest4_Data

#include <vector>

class Data
{
	private:
		std::vector<double> t, y, sig;

	public:
		Data();
		void load(const char* filename);

		// Getters
		const std::vector<double>& get_t() const { return t; }
		const std::vector<double>& get_y() const { return y; }
        const std::vector<double>& get_sig() const { return sig; }

	// Singleton
	private:
		static Data instance;
	public:
		static Data& get_instance() { return instance; }
};

#endif

