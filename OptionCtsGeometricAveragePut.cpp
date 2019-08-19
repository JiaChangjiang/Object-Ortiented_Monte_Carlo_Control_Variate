//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	OptionCtsGeometricAverageCall.cpp
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

#include "OptionCtsGeometricAveragePut.h"

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

template<> std::string ClassName<OptionCtsGeometricAveragePut>()
{
	return "OptionCtsGeometricAveragePut";
}

template<> std::string ClassID<OptionCtsGeometricAveragePut>()
{
	return "asian_geo_p";
}

template<> std::string BaseClassName<OptionCtsGeometricAveragePut>()
{
	return ClassName<OptionBase>();
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  register with factory
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

namespace
{
	AppFactoryRegistration<OptionBase, OptionCtsGeometricAveragePut> 
													OptionCtsGeometricAveragePut;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	constructor
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

OptionCtsGeometricAveragePut::OptionCtsGeometricAveragePut()
:	 X_(IO_Manager::Instance().GetValue<double>(ParameterX()))
,	 T_(IO_Manager::Instance().GetValue<double>(ParameterT()))
{}

OptionCtsGeometricAveragePut::OptionCtsGeometricAveragePut(double X, double T)
:	 X_(X)
,	 T_(T)
{}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	Compute Payoff
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

double OptionCtsGeometricAveragePut::ComputePayoff(std::vector<double> & path) const
{
	double av = AverageGeometricFirstExcluded(path);

	return ComputePO(av);
}

double OptionCtsGeometricAveragePut::ComputePayoff
							(std::vector<double> & path, Control_Variate &) const
{
	return ComputePayoff(path);
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	ComputePO()
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

double OptionCtsGeometricAveragePut::ComputePO(const double S) const
{
	return std::max(X_ - S, 0.0);
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	Compute GeometricAverage
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

double OptionCtsGeometricAveragePut::AverageGeometricFirstExcluded
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

std::string OptionCtsGeometricAveragePut::GetName() const 
							{ return ClassName<OptionCtsGeometricAveragePut>(); }
std::string OptionCtsGeometricAveragePut::GetID() const 
							{ return ClassID<OptionCtsGeometricAveragePut>(); }
std::string OptionCtsGeometricAveragePut::GetBaseClassName() const 
						{ return BaseClassName<OptionCtsGeometricAveragePut>(); }
std::string OptionCtsGeometricAveragePut::GetStringify() const
{
	std::string nm = GetBaseClassName() + " is " + GetName() + '\n';
	nm += "  X = " + ut::ConvertToString<double>(X_) + '\n';
	nm += "  T = " + ut::ConvertToString<double>(T_) + '\n';
	return nm;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	end
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
