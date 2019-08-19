//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  ProcesGBM.h
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
#ifndef ProcessGBMH
#define ProcessGBMH
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

class DeterministicVolatilityBase;
class StochasticVolatilityBase;

#include <vector>

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  class   ProcessGBM
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

class ProcessGBM
{
	public:
		ProcessGBM();
		~ProcessGBM();

		void Initialise(DeterministicVolatilityBase& Deterministic_Volatility_Base);
		void Initialise(StochasticVolatilityBase& Stochastic_Volatility_Base);
		double Next_Y(double Present_Y, long i, double dw1);
		double Next_V(double Present_Y);
	
		double Next_S(double S, long i, double dw1, double Present_V);
	
		void FillUpPath(std::vector<double> & path, std::vector<double> & random_number);
	
		//auxiliary process
		void FillUpPath(std::vector<double> & path, std::vector<double>& dW1, DeterministicVolatilityBase&);
	
		double Next_V(long i, DeterministicVolatilityBase&);
	
	private:
		double S_0_;
		double r_;

		double Y0_;
	
		double dt_;
		
		DeterministicVolatilityBase * Deterministic_Volatility_Base_;
		StochasticVolatilityBase * Stochastic_Volatility_Base_;
};

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
#endif
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  End
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
