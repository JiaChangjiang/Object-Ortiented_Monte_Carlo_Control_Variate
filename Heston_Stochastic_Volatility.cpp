//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	Heston_Stochastic_Volality.cpp
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

#include "Heston_Stochastic_Volatility.h"

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
#include "ParameterTheta.h"
#include "ParameterK.h"
#include "ParameterY0.h"

#include "ParameterN.h"
#include "ParameterT.h"

#include <cmath>

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  createable
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

template<> std::string ClassName<Heston_Stochastic_Volatility>()
{
	return "Heston_Stochastic_Volatility";
}

template<> std::string ClassID<Heston_Stochastic_Volatility>()
{
	return "Heston";
}

template<> std::string BaseClassName<Heston_Stochastic_Volatility>()
{
	return ClassName<StochasticVolatilityBase>();
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  register with factory
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

namespace
{
	AppFactoryRegistration<StochasticVolatilityBase, Heston_Stochastic_Volatility>
											RegisterHeston_Stochastic_Volatility;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  Constructor
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

Heston_Stochastic_Volatility::Heston_Stochastic_Volatility()
:	S_0_(IO_Manager::Instance().GetValue<double>(ParameterS0()))
,	r_(IO_Manager::Instance().GetValue<double>(ParameterR()))
,	V_0_(IO_Manager::Instance().GetValue<double>(ParameterY0()))
,	heston_rho_(IO_Manager::Instance().GetValue<double>(ParameterRho()))
,	heston_k_(IO_Manager::Instance().GetValue<double>(ParameterK()))
,	heston_theta_(IO_Manager::Instance().GetValue<double>(ParameterTheta()))
,	heston_sigma_(IO_Manager::Instance().GetValue<double>(ParameterSigma()))
{
	double T = IO_Manager::Instance().GetValue<double>(ParameterT());
	double N = IO_Manager::Instance().GetValue<long>(ParameterN());
	dt_ = T / N;
}

Heston_Stochastic_Volatility::~Heston_Stochastic_Volatility()
{}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	compute next Y and V
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

double Heston_Stochastic_Volatility::Next_Y(double Present_Y, long i, double dw1)
{
	double dw2 = rv::GetNormalVariate();
	double z2 = heston_rho_ * dw1 + dw2 * std::sqrt(1.0 - heston_rho_ * heston_rho_);

	double m = Present_Y * std::exp(-heston_k_ * dt_) + heston_theta_ 
												* (1 - std::exp(-heston_k_ * dt_));
	
	double s_2 = Present_Y * heston_sigma_*heston_sigma_ / heston_k_ 
					* (std::exp(-heston_k_ * dt_) - std::exp(-2 * heston_k_*dt_)) 
					+ heston_theta_ * heston_sigma_*heston_sigma_
					* (1 - std::exp(-heston_k_ * dt_)) 
					* (1 - std::exp(-heston_k_ * dt_)) / (2 * heston_k_);
					
	double n = log(1 + s_2 / m / m);
	//n is the sigma square in the mock solution
	
	return  m * std::exp(-0.5 * n + std::sqrt(n) * z2);
}

double Heston_Stochastic_Volatility::Next_V(double Present_Y)
{
	return Present_Y;	
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	CreatableBase stuff
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

std::string Heston_Stochastic_Volatility::GetName() const 
							{ return ClassName<Heston_Stochastic_Volatility>(); }
std::string Heston_Stochastic_Volatility::GetID() const 
							{ return ClassID<Heston_Stochastic_Volatility>(); }
std::string Heston_Stochastic_Volatility::GetBaseClassName() const 
						{ return BaseClassName<Heston_Stochastic_Volatility>(); }
std::string Heston_Stochastic_Volatility::GetStringify() const
{
	std::string nm = GetBaseClassName() + " is " + GetName() + '\n';
	nm += "  S0 = " + ut::ConvertToString<double>(S_0_) + '\n';
	nm += "  V0 = " + ut::ConvertToString<double>(V_0_) + '\n';
	return nm;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	end
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
