//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	OptionEuroPut.cpp
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

#include "OptionEuroPut.h"

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

template<> std::string ClassName<OptionEuroPut>()
{
	return "OptionEuroPut";
}

template<> std::string ClassID<OptionEuroPut>()
{
	return "p";
}

template<> std::string BaseClassName<OptionEuroPut>()
{
	return ClassName<OptionBase>();
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  register with factory
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

namespace
{
	AppFactoryRegistration<OptionBase, OptionEuroPut> RegisterOptionEuroCall;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	constructor
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

OptionEuroPut::OptionEuroPut()
:	X_(IO_Manager::Instance().GetValue<double>(ParameterX()))
,	T_(IO_Manager::Instance().GetValue<double>(ParameterT()))
{}

OptionEuroPut::~OptionEuroPut()
{}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	Compute payoff
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

double OptionEuroPut::ComputePO(double S) const
{
	return std::max(0.0, X_ - S);
}

double OptionEuroPut::ComputePayoff(std::vector<double>& path) const
{
	long N = long(path.size()) - 1;

	return ComputePO(path[N]);
}

double OptionEuroPut::ComputePayoff
							(std::vector<double>& path, Control_Variate&) const
{
	return ComputePayoff(path);
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	Compute Explicit
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

double OptionEuroPut::AuxiliaryExplicit(Deterministic_Vol_NM_Heston &)
{
	double S_0 = IO_Manager::Instance().GetValue<double>(ParameterS0());
	double X = IO_Manager::Instance().GetValue<double>(ParameterX());
	double y_0 = IO_Manager::Instance().GetValue<double>(ParameterY0());
	double r = IO_Manager::Instance().GetValue<double>(ParameterR());
	double T = IO_Manager::Instance().GetValue<double>(ParameterT());
	double k = IO_Manager::Instance().GetValue<double>(ParameterK());
	double theta = IO_Manager::Instance().GetValue<double>(ParameterTheta());

	return af::Euro_put_Heston_NewMethod(S_0, r, X, T, y_0, k, theta);
}

double OptionEuroPut::AuxiliaryExplicit(Deterministic_Vol_MI_Heston &)
{
	double S_0 = IO_Manager::Instance().GetValue<double>(ParameterS0());
	double X = IO_Manager::Instance().GetValue<double>(ParameterX());
	double y_0 = IO_Manager::Instance().GetValue<double>(ParameterY0());
	double r = IO_Manager::Instance().GetValue<double>(ParameterR());
	double T = IO_Manager::Instance().GetValue<double>(ParameterT());
	double k = IO_Manager::Instance().GetValue<double>(ParameterK());
	double theta = IO_Manager::Instance().GetValue<double>(ParameterTheta());

	return af::Euro_put_Heston_MethodI(S_0, r, X, T, y_0, k, theta);
}

double OptionEuroPut::AuxiliaryExplicit(Deterministic_Vol_MII_Heston &)
{
	double S_0 = IO_Manager::Instance().GetValue<double>(ParameterS0());
	double X = IO_Manager::Instance().GetValue<double>(ParameterX());
	double y_0 = IO_Manager::Instance().GetValue<double>(ParameterY0());
	double r = IO_Manager::Instance().GetValue<double>(ParameterR());
	double T = IO_Manager::Instance().GetValue<double>(ParameterT());
	double k = IO_Manager::Instance().GetValue<double>(ParameterK());
	double theta = IO_Manager::Instance().GetValue<double>(ParameterTheta());

	return af::Euro_put_Heston_MethodII(S_0, r, X, T, y_0, k, theta);
}

double OptionEuroPut::AuxiliaryExplicit(Deterministic_Vol_NM_HW &)
{
	double S_0 = IO_Manager::Instance().GetValue<double>(ParameterS0());
	double X = IO_Manager::Instance().GetValue<double>(ParameterX());
	double y_0 = IO_Manager::Instance().GetValue<double>(ParameterY0());
	double r = IO_Manager::Instance().GetValue<double>(ParameterR());
	double T = IO_Manager::Instance().GetValue<double>(ParameterT());
	double mu = IO_Manager::Instance().GetValue<double>(ParameterMiu());
	double m = IO_Manager::Instance().GetValue<double>(ParameterHWM());
	double hull_white_sig = IO_Manager::Instance().GetValue<double>(ParameterHWSigma());

	return af::Euro_put_HullWhite_NewMethod(S_0, r, X, T, mu, m, hull_white_sig, y_0);
}

double OptionEuroPut::AuxiliaryExplicit(Deterministic_Vol_MI_HW &)
{
	double S_0 = IO_Manager::Instance().GetValue<double>(ParameterS0());
	double X = IO_Manager::Instance().GetValue<double>(ParameterX());
	double y_0 = IO_Manager::Instance().GetValue<double>(ParameterY0());
	double r = IO_Manager::Instance().GetValue<double>(ParameterR());
	double T = IO_Manager::Instance().GetValue<double>(ParameterT());
	double mu = IO_Manager::Instance().GetValue<double>(ParameterMiu());
	double m = IO_Manager::Instance().GetValue<double>(ParameterHWM());
	double hull_white_sig = IO_Manager::Instance().GetValue<double>(ParameterHWSigma());

	return af::Euro_put_HullWhite_MethodI(S_0, r, X, T, mu, m, hull_white_sig, y_0);
}

double OptionEuroPut::AuxiliaryExplicit(Deterministic_Vol_MII_HW &)
{
	double S_0 = IO_Manager::Instance().GetValue<double>(ParameterS0());
	double X = IO_Manager::Instance().GetValue<double>(ParameterX());
	double y_0 = IO_Manager::Instance().GetValue<double>(ParameterY0());
	double r = IO_Manager::Instance().GetValue<double>(ParameterR());
	double T = IO_Manager::Instance().GetValue<double>(ParameterT());
	double mu = IO_Manager::Instance().GetValue<double>(ParameterMiu());
	double m = IO_Manager::Instance().GetValue<double>(ParameterHWM());
	double hull_white_sig = IO_Manager::Instance().GetValue<double>(ParameterHWSigma());

	return af::Euro_put_HullWhite_MethodII(S_0, r, X, T, mu, m, hull_white_sig, y_0);
}

double OptionEuroPut::AuxiliaryExplicit(Deterministic_Vol_NM_Stein &)
{
	double S_0 = IO_Manager::Instance().GetValue<double>(ParameterS0());
	double X = IO_Manager::Instance().GetValue<double>(ParameterX());
	double y_0 = IO_Manager::Instance().GetValue<double>(ParameterY0());
	double r = IO_Manager::Instance().GetValue<double>(ParameterR());
	double T = IO_Manager::Instance().GetValue<double>(ParameterT());
	double alpha = IO_Manager::Instance().GetValue<double>(ParameterAlpha());
	double beta = IO_Manager::Instance().GetValue<double>(ParameterBeta());

	return af::Euro_put_Steinstein_NewMethod(S_0, r, X, T, y_0, alpha, beta);
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	CreatableBase stuff
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

std::string OptionEuroPut::GetName() const { return ClassName<OptionEuroPut>(); }
std::string OptionEuroPut::GetID() const { return ClassID<OptionEuroPut>(); }
std::string OptionEuroPut::GetBaseClassName() const 
									   { return BaseClassName<OptionEuroPut>(); }
std::string OptionEuroPut::GetStringify() const
{
	std::string nm = GetBaseClassName() + " is " + GetName() + '\n';
	nm += "  X = " + ut::ConvertToString<double>(X_) + '\n';
	nm += "  T = " + ut::ConvertToString<double>(T_) + '\n';
	return nm;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	end
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
