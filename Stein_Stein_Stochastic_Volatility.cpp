//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	Stein_Stein_Stochastic_Volatility.cpp
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

#include "Stein_Stein_Stochastic_Volatility.h"

#include "DeterministicVolatilityBase.h"
#include "rv_library.h"
#include "utility.h"

#include "CreateableBase.h"
#include "IO_Manager.h"

#include "AppFactoryRegistration.h"

#include "ParameterS0.h"
#include "ParameterR.h"
#include "ParameterSigma.h"
#include "ParameterRho.h"
#include "ParameterAlpha.h"
#include "ParameterBeta.h"
#include "ParameterY0.h"

#include "ParameterN.h"
#include "ParameterT.h"

#include <cmath>

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  createable
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

template<> std::string ClassName<Stein_Stein_Stochastic_Volatility>()
{
	return "Stein_Stein_Stochastic_Volatility";
}

template<> std::string ClassID<Stein_Stein_Stochastic_Volatility>()
{
	return "Stein";
}

template<> std::string BaseClassName<Stein_Stein_Stochastic_Volatility>()
{
	return ClassName<StochasticVolatilityBase>();
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  register with factory
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

namespace
{
	AppFactoryRegistration<StochasticVolatilityBase, Stein_Stein_Stochastic_Volatility>
											RegisterStein_Stein_Stochastic_Volatility;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  Constructor
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

Stein_Stein_Stochastic_Volatility::Stein_Stein_Stochastic_Volatility()
:	S_0_(IO_Manager::Instance().GetValue<double>(ParameterS0()))
,	r_(IO_Manager::Instance().GetValue<double>(ParameterR()))

,	steinstein_sig_(IO_Manager::Instance().GetValue<double>(ParameterSigma()))
,	steinstein_rho_(IO_Manager::Instance().GetValue<double>(ParameterRho()))
,	steinstein_alpha_(IO_Manager::Instance().GetValue<double>(ParameterAlpha()))

,	steinstein_beta_(IO_Manager::Instance().GetValue<double>(ParameterBeta()))
,	stein_stein_Y0_(IO_Manager::Instance().GetValue<double>(ParameterY0()))
{
	double T = IO_Manager::Instance().GetValue<double>(ParameterT());
	double N = IO_Manager::Instance().GetValue<long>(ParameterN());
	dt_ = T / N;
}

Stein_Stein_Stochastic_Volatility::~Stein_Stein_Stochastic_Volatility()
{}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	compute next Y and V
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

double Stein_Stein_Stochastic_Volatility::Next_Y(double Present_Y, long i, double dw1)
{
	double dw2 = rv::GetNormalVariate();
	double z2 = steinstein_rho_ * dw1 + dw2 * sqrt(1 - steinstein_rho_ * steinstein_rho_);

	double m = Present_Y * exp( -steinstein_alpha_ * dt_) + steinstein_beta_ 
											* (1 - exp(-steinstein_alpha_ * dt_));
	double s_2 = steinstein_sig_ * steinstein_sig_ / 2 / steinstein_alpha_ 
										* (1 - exp(-2 * steinstein_alpha_ * dt_));
	double n = log(1 + s_2 / m / m);
	double Y = m * exp( - 0.5 * n + std::sqrt(n) * z2 );
	
	return Y;	
}

double Stein_Stein_Stochastic_Volatility::Next_V(double Present_Y)
{
	return Present_Y * Present_Y;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	CreatableBase stuff
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

std::string Stein_Stein_Stochastic_Volatility::GetName() const 
						{ return ClassName<Stein_Stein_Stochastic_Volatility>(); }
std::string Stein_Stein_Stochastic_Volatility::GetID() const 
						{ return ClassID<Stein_Stein_Stochastic_Volatility>(); }
std::string Stein_Stein_Stochastic_Volatility::GetBaseClassName() const 
					{ return BaseClassName<Stein_Stein_Stochastic_Volatility>(); }
std::string Stein_Stein_Stochastic_Volatility::GetStringify() const
{
	std::string nm = GetBaseClassName() + " is " + GetName() + '\n';
	nm += "  S0 = " + ut::ConvertToString<double>(S_0_) + '\n';
	nm += "  Y0 = " + ut::ConvertToString<double>(stein_stein_Y0_) + '\n';
	return nm;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	end
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
