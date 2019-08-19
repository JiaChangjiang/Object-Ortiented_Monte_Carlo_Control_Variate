//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	Hull_White_Stochastic_Volatility.cpp
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

#include "Hull_White_Stochastic_Volatility.h"

#include"DeterministicVolatilityBase.h"
#include"rv_library.h"
#include "utility.h"

#include "CreateableBase.h"
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
//  createable
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

template<> std::string ClassName<Hull_White_Stochastic_Volatility>()
{
	return "Hull_White_Stochastic_Volatility";
}

template<> std::string ClassID<Hull_White_Stochastic_Volatility>()
{
	return "HW";
}

template<> std::string BaseClassName<Hull_White_Stochastic_Volatility>()
{
	return ClassName<StochasticVolatilityBase>();
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  register with factory
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

namespace
{
	AppFactoryRegistration<StochasticVolatilityBase, Hull_White_Stochastic_Volatility>
											RegisterHull_White_Stochastic_Volatility;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  Constructor
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

Hull_White_Stochastic_Volatility::Hull_White_Stochastic_Volatility()
:	S_0_(IO_Manager::Instance().GetValue<double>(ParameterS0()))
,	r_(IO_Manager::Instance().GetValue<double>(ParameterR()))
,	hullwhite_sig_(IO_Manager::Instance().GetValue<double>(ParameterHWSigma()))
,	hullwhite_rho_(IO_Manager::Instance().GetValue<double>(ParameterRho()))
,	hullwhite_miu_(IO_Manager::Instance().GetValue<double>(ParameterMiu()))
,	hullwhite_Y0_(IO_Manager::Instance().GetValue<double>(ParameterY0()))
{
	double T = IO_Manager::Instance().GetValue<double>(ParameterT());
	double N = IO_Manager::Instance().GetValue<long>(ParameterN());
	dt_ = T / N;
}

Hull_White_Stochastic_Volatility::~Hull_White_Stochastic_Volatility()
{}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	comoute next Y and V
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

double Hull_White_Stochastic_Volatility::Next_Y(double Present_Y, long i, double dw1)
{
	double dw2 = rv::GetNormalVariate();
	double z2 = hullwhite_rho_ * dw1 + dw2 * sqrt(1.0 - hullwhite_rho_ * hullwhite_rho_);
	double drift_term = (hullwhite_miu_ - 0.5 * hullwhite_sig_ * hullwhite_sig_) * dt_;
	double random_term = hullwhite_sig_ * z2 * sqrt(dt_);
	
	Present_Y = Present_Y * (exp(drift_term + random_term));
	
	return Present_Y; 
}

double Hull_White_Stochastic_Volatility::Next_V(double Present_Y)
{
	return Present_Y;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	CreatableBase stuff
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

std::string Hull_White_Stochastic_Volatility::GetName() const 
						{ return ClassName<Hull_White_Stochastic_Volatility>(); }
std::string Hull_White_Stochastic_Volatility::GetID() const 
						{ return ClassID<Hull_White_Stochastic_Volatility>(); }
std::string Hull_White_Stochastic_Volatility::GetBaseClassName() const 
					{ return BaseClassName<Hull_White_Stochastic_Volatility>(); }
std::string Hull_White_Stochastic_Volatility::GetStringify() const
{
	std::string nm = GetBaseClassName() + " is " + GetName() + '\n';
	nm += "  S0 = " + ut::ConvertToString<double>(S_0_) + '\n';
	nm += "  Y0 = " + ut::ConvertToString<double>(hullwhite_Y0_) + '\n';
	return nm;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	end
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
