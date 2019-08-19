//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	OptionCtsGeometricAverageCall.cpp
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

#include "OptionCtsGeometricAverageCall.h"

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

template<> std::string ClassName<OptionCtsGeometricAverageCall>()
{
	return "OptionCtsGeometricAverageCall";
}

template<> std::string ClassID<OptionCtsGeometricAverageCall>()
{
	return "asian_geo_c";
}

template<> std::string BaseClassName<OptionCtsGeometricAverageCall>()
{
	return ClassName<OptionBase>();
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  register with factory
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

namespace
{
	AppFactoryRegistration<OptionBase, OptionCtsGeometricAverageCall> 
													OptionCtsGeometricAverageCall;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	constructor
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

OptionCtsGeometricAverageCall::OptionCtsGeometricAverageCall()
:	 X_(IO_Manager::Instance().GetValue<double>(ParameterX()))
,	 T_(IO_Manager::Instance().GetValue<double>(ParameterT()))
{}

OptionCtsGeometricAverageCall::OptionCtsGeometricAverageCall(double X, double T)
:	 X_(X)
,	 T_(T)
{}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	Compute Payoff
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

double OptionCtsGeometricAverageCall::ComputePayoff(std::vector<double> & path) const
{
	double av = AverageGeometricFirstExcluded(path);

	return ComputePO(av);
}

double OptionCtsGeometricAverageCall::ComputePayoff
							(std::vector<double> & path, Control_Variate &) const
{
	return ComputePayoff(path);
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	get explicit
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

double OptionCtsGeometricAverageCall::AuxiliaryExplicit(Deterministic_Vol_NM_HW &)
{
	{
		double S_0 = IO_Manager::Instance().GetValue<double>(ParameterS0());
		double X = IO_Manager::Instance().GetValue<double>(ParameterX());
		double y_0 = IO_Manager::Instance().GetValue<double>(ParameterY0());
		double r = IO_Manager::Instance().GetValue<double>(ParameterR());
		double T = IO_Manager::Instance().GetValue<double>(ParameterT());
		double mu = IO_Manager::Instance().GetValue<double>(ParameterMiu());
		double m = IO_Manager::Instance().GetValue<double>(ParameterHWM());
		double hull_white_sig = IO_Manager::Instance().GetValue<double>(ParameterHWSigma());

		return af::Asian_call_HullWhite_NewMethod(S_0, r, X, T, y_0, mu, hull_white_sig, m);
	}
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	ComputePO()
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

double OptionCtsGeometricAverageCall::ComputePO(const double S) const
{
	return std::max(S - X_, 0.0);
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	Compute GeometricAverage
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

double OptionCtsGeometricAverageCall::AverageGeometricFirstExcluded
												(std::vector<double> & path) const
{
	long N = long(path.size()) - 1;
	double av = 0;
	double StartIndex = 1;// exclude first number
	
	for (long i = StartIndex; i <= N; ++i)
	{
		av += std::log(path[i]);
	}
	
	return std::exp(av / N);
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	CreatableBase stuff
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

std::string OptionCtsGeometricAverageCall::GetName() const 
							{ return ClassName<OptionCtsGeometricAverageCall>(); }
std::string OptionCtsGeometricAverageCall::GetID() const 
							{ return ClassID<OptionCtsGeometricAverageCall>(); }
std::string OptionCtsGeometricAverageCall::GetBaseClassName() const 
						{ return BaseClassName<OptionCtsGeometricAverageCall>(); }
std::string OptionCtsGeometricAverageCall::GetStringify() const
{
	std::string nm = GetBaseClassName() + " is " + GetName() + '\n';
	nm += "  X = " + ut::ConvertToString<double>(X_) + '\n';
	nm += "  T = " + ut::ConvertToString<double>(T_) + '\n';
	return nm;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	end
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
