//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	OptionCtsAveragePut.cpp
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

#include "OptionCtsAveragePut.h"
#include"OptionCtsGeometricAveragePut.h"

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

template<> std::string ClassName<OptionCtsAveragePut>()
{
	return "OptionCtsAveragePut";
}

template<> std::string ClassID<OptionCtsAveragePut>()
{
	return "asian_ari_p";
}

template<> std::string BaseClassName<OptionCtsAveragePut>()
{
	return ClassName<OptionBase>();
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  register with factory
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

namespace
{
	AppFactoryRegistration<OptionBase, OptionCtsAveragePut> 
													RegisterOptionCtsAveragePut;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	constructor
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

OptionCtsAveragePut::OptionCtsAveragePut()
:	 X_(IO_Manager::Instance().GetValue<double>(ParameterX()))
,	 T_(IO_Manager::Instance().GetValue<double>(ParameterT()))
{
	Geo_asian_ = new OptionCtsGeometricAveragePut(X_, T_);
	// For arithmetic asain option, there should be auxiliary geometric asain option with same parameters to 
	// compute explicit solution and provide auxiliary process sol.
}

OptionCtsAveragePut::~OptionCtsAveragePut()
{
	delete Geo_asian_;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	compute payoff
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

double OptionCtsAveragePut::ComputePO(double S) const
{
	return std::max(0.0, X_ - S);
}

double OptionCtsAveragePut::ComputePayoff(std::vector<double> & path) const
{
	double av = AverageArithmeticFirstExcluded(path);

	return ComputePO(av);
}

double OptionCtsAveragePut::ComputePayoff 
							(std::vector<double> & path, Control_Variate &) const
{ 
    //OptionCtsGeometricAverageCall Aux_Option(X_,T_);
	//return Aux_Option.ComputePayoff(path);
	return Geo_asian_->ComputePayoff(path);
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	Compute ArithmetricAverage
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

double OptionCtsAveragePut::AverageArithmeticFirstExcluded
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
//	CreatableBase stuff
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

std::string OptionCtsAveragePut::GetName() const 
									{ return ClassName<OptionCtsAveragePut>(); }
std::string OptionCtsAveragePut::GetID() const 
									{ return ClassID<OptionCtsAveragePut>(); }
std::string OptionCtsAveragePut::GetBaseClassName() const 
								{ return BaseClassName<OptionCtsAveragePut>(); }
std::string OptionCtsAveragePut::GetStringify() const
{
	std::string nm = GetBaseClassName() + " is " + GetName() + '\n';
	nm += "  X = " + ut::ConvertToString<double>(X_) + '\n';
	nm += "  T = " + ut::ConvertToString<double>(T_) + '\n';
	return nm;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	end
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
