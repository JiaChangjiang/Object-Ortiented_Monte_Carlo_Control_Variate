//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	OptionCtsAverageCall.cpp
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

#include "OptionCtsAverageCall.h"
#include"OptionCtsGeometricAverageCall.h"

#include "utility.h"
#include "rv_library.h"

#include "AnalyticalFormule.h"

#include "CreateableBase.h"

#include "IO_Manager.h"
#include "AppFactoryRegistration.h"

#include "ParameterX.h"
#include "ParameterT.h"
#include "ParameterS0.h"
#include "ParameterR.h"
#include "ParameterSigma.h"
#include "ParameterRho.h"
#include "ParameterY0.h"

#include "ParameterHWSigma.h"
#include "ParameterMiu.h"
#include "ParameterHWM.h"

#include "ParameterK.h"
#include "ParameterTheta.h"

#include "ParameterAlpha.h"
#include "ParameterBeta.h"

#include <algorithm>

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  createable
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

template<> std::string ClassName<OptionCtsAverageCall>()
{
	return "OptionCtsAverageCall";
}

template<> std::string ClassID<OptionCtsAverageCall>()
{
	return "asian_ari_c";
}

template<> std::string BaseClassName<OptionCtsAverageCall>()
{
	return ClassName<OptionBase>();
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  register with factory
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

namespace
{
	AppFactoryRegistration<OptionBase, OptionCtsAverageCall> 
													RegisterOptionCtsAverageCall;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	constructor
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

OptionCtsAverageCall::OptionCtsAverageCall()
:	 X_(IO_Manager::Instance().GetValue<double>(ParameterX()))
,	 T_(IO_Manager::Instance().GetValue<double>(ParameterT()))
{
	Geo_asian_ = new OptionCtsGeometricAverageCall(X_, T_);
	// For arithmetic asain option, there should be auxiliary geometric asain option with same parameters to 
	// compute explicit solution and provide auxiliary process sol.
}

OptionCtsAverageCall::~OptionCtsAverageCall()
{
	delete Geo_asian_;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	compute payoff
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

double OptionCtsAverageCall::ComputePO(double S) const
{
	return std::max(0.0, S - X_);
}

double OptionCtsAverageCall::ComputePayoff(std::vector<double> & path) const
{
	double av = AverageArithmeticFirstExcluded(path);

	return ComputePO(av);
}

double OptionCtsAverageCall::ComputePayoff 
							(std::vector<double> & path, Control_Variate &) const
{ 
    //OptionCtsGeometricAverageCall Aux_Option(X_,T_);
	//return Aux_Option.ComputePayoff(path);
	return Geo_asian_->ComputePayoff(path);
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	Compute ArithmetricAverage
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

double OptionCtsAverageCall::AverageArithmeticFirstExcluded
											   (std::vector<double> & path) const
{ 
	double N = long(path.size())-1;
	double av = 0;
	double StartIndex = 1;// exclude first number
	
	for (long i = StartIndex; i <= N ; i++)
	{
		av += path[i];
	}
	
	return av / N;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	Compute Explicit
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

double OptionCtsAverageCall::AuxiliaryExplicit(Deterministic_Vol_NM_HW & t)
{
	return Geo_asian_->AuxiliaryExplicit(t);
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	CreatableBase stuff
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

std::string OptionCtsAverageCall::GetName() const 
									{ return ClassName<OptionCtsAverageCall>(); }
std::string OptionCtsAverageCall::GetID() const 
									{ return ClassID<OptionCtsAverageCall>(); }
std::string OptionCtsAverageCall::GetBaseClassName() const 
								{ return BaseClassName<OptionCtsAverageCall>(); }
std::string OptionCtsAverageCall::GetStringify() const
{
	std::string nm = GetBaseClassName() + " is " + GetName() + '\n';
	nm += "  X = " + ut::ConvertToString<double>(X_) + '\n';
	nm += "  T = " + ut::ConvertToString<double>(T_) + '\n';
	return nm;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	end
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
