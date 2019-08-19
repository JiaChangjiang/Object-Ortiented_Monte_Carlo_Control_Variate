//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
// Stein_Stein_Stochastic_Volatility.h
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
#ifndef Stein_Stein_Stochastic_VolatilityH
#define Stein_Stein_Stochastic_VolatilityH
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

#include "StochasticVolatilityBase.h"

//XXXXXXXdXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  class   Stein_Stein_Process
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

class Stein_Stein_Stochastic_Volatility : public StochasticVolatilityBase
{
	public:
		Stein_Stein_Stochastic_Volatility();
		~Stein_Stein_Stochastic_Volatility();
			
		double Next_Y(double Present_Y, long i, double dw1);
		double Next_V(double Present_Y);
	
		// CreatableBase stuff
		std::string GetName() const;
		std::string GetID() const;
		std::string GetBaseClassName() const;
		std::string GetStringify() const;	
		
	private:
		double S_0_;
		double r_;
	
		double steinstein_sig_;
		double steinstein_rho_;
		double steinstein_alpha_;
		double steinstein_beta_;
		double stein_stein_Y0_;
	
		double dt_;
};

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
#endif
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  End
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
