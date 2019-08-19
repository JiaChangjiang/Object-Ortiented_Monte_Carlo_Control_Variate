//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	ProcessGBM.cpp
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

#include "ProcessGBM.h"
#include "DeterministicVolatilityBase.h"
#include "StochasticVolatilityBase.h"

#include "rv_library.h"
#include "utility.h"

#include "IO_Manager.h"

#include "AppFactoryRegistration.h"

#include "ParameterS0.h"
#include "ParameterR.h"
#include "ParameterSigma.h"
#include "ParameterRho.h"
#include "ParameterMiu.h"
#include "ParameterY0.h"
#include "ParameterHWSigma.h"

#include "ParameterN.h"
#include "ParameterT.h"

#include <cmath>

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	constructor
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

ProcessGBM::ProcessGBM( )
:	 S_0_(IO_Manager::Instance().GetValue<double>(ParameterS0()))
,	 r_(IO_Manager::Instance().GetValue<double>(ParameterR()))
,	 Y0_(IO_Manager::Instance().GetValue<double>(ParameterY0()))
{
	double T = IO_Manager::Instance().GetValue<double>(ParameterT());
	double N = IO_Manager::Instance().GetValue<long>(ParameterN());
	dt_ = T / N;	
}

ProcessGBM::~ProcessGBM()
{}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	MC stuff
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

void ProcessGBM::Initialise
					(DeterministicVolatilityBase & Deterministic_Volatility_Base)
{
	Deterministic_Volatility_Base_ = &Deterministic_Volatility_Base;
}

void ProcessGBM::Initialise(StochasticVolatilityBase & Stochastic_Volatility_Base)
{
	Stochastic_Volatility_Base_ = &Stochastic_Volatility_Base;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
// get next volatility from stochastic volatility
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

double ProcessGBM::Next_Y(double Present_Y, long i, double dw1)
{
	return Stochastic_Volatility_Base_->Next_Y(Present_Y, i, dw1);
}

double ProcessGBM::Next_V(double Present_Y)
{
	return Stochastic_Volatility_Base_->Next_V(Present_Y);
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
// get stock price of the next time step
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

double ProcessGBM::Next_S(double S, long i, double dw1, double Present_V)
{
	double drift_term = (r_ - 0.5 * Present_V) * dt_;
	double random_term = std::sqrt(Present_V) * dw1 * std::sqrt(dt_);
	return S * std::exp(drift_term + random_term);
}

void ProcessGBM::FillUpPath
				(std::vector<double>& path, std::vector<double> & random_number)
{
	long N = long(path.size()) - 1;

	double Present_Y = Y0_;
	double Present_V = Next_V(Y0_);

	path[0] = S_0_;

	for (long i = 1; i <= N; ++i)

	{
		double dw1 = random_number.at(i - 1);
		path[i] = Next_S(path[i - 1], i - 1, dw1, Present_V);
		Present_Y = Next_Y(Present_Y, i - 1, dw1);
		Present_V = Next_V(Present_Y);
	}
}

void ProcessGBM::FillUpPath(std::vector<double> & path, std::vector<double> & dW1, DeterministicVolatilityBase&)
{
	long N = long(path.size()) - 1;
	double Present_V = Next_V(0, *Deterministic_Volatility_Base_);
	path[0] = S_0_;
	
	for (long i = 1; i <= N; ++i)
	{
		path[i] = Next_S(path[i - 1], i - 1, dW1[i - 1], Present_V);
		Present_V = Next_V(i, *Deterministic_Volatility_Base_);
	}
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
// get next volatility from deterministic volatility
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

double ProcessGBM::Next_V(long i, DeterministicVolatilityBase&)
{
	//need to check the return value is y or volatilit	
	return Deterministic_Volatility_Base_->Next_V(i);
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	end
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
