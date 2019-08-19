//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	Deterministic_Vol_NM_Stein.cpp
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

#include "Deterministic_Vol_NM_Stein.h"

#include "OptionBase.h"

#include "IO_Manager.h"
#include "AppFactoryRegistration.h"
#include "CreateableBase.h"

#include "ParameterY0.h"
#include "ParameterT.h"
#include "ParameterN.h"
#include "ParameterAlpha.h"
#include "ParameterBeta.h"

#include "utility.h"
#include "rv_library.h"

#include <cmath>
#include <string>
#include <iostream>

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  createable
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

template<> std::string ClassName<Deterministic_Vol_NM_Stein>()
{
	return "Deterministic_Vol_NM_Stein";
}

template<> std::string ClassID<Deterministic_Vol_NM_Stein>()
{
	return "NMStein";
}

template<> std::string BaseClassName<Deterministic_Vol_NM_Stein>()
{
	return ClassName<DeterministicVolatilityBase>();
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  register with factory
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

namespace
{
	AppFactoryRegistration<DeterministicVolatilityBase, Deterministic_Vol_NM_Stein> 
												RegisterDeterministic_Vol_NM_Stein;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	constructor
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

Deterministic_Vol_NM_Stein::Deterministic_Vol_NM_Stein()
:	 N_(IO_Manager::Instance().GetValue<double>(ParameterN()))
,	 T_(IO_Manager::Instance().GetValue<double>(ParameterT()))
,	 alpha_(IO_Manager::Instance().GetValue<double>(ParameterAlpha()))
,	 beta_(IO_Manager::Instance().GetValue<double>(ParameterBeta()))
,	 Y0_(IO_Manager::Instance().GetValue<double>(ParameterY0()))
{
	dt_ = T_ / N_;
	
	V_.resize(N_ + 1);
	V_[0] = Y0_ * Y0_;
	
	// compute the deterministic volatility at each time step and save as a vector
	for (long i = 1; i <= N_; ++i)
	{
		V_[i] = exp(-alpha_ * dt_ * i) * Y0_ + beta_ * (1 - exp(-alpha_ * dt_ * i));
		V_[i] = V_[i] * V_[i];
	}
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	get next V
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

double Deterministic_Vol_NM_Stein::Next_V(double i)
{
	return V_[ i ];
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	get explicit
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

bool Deterministic_Vol_NM_Stein::HasExplicit(OptionBase & opt)
{
	return opt.HasExplicit(*this);
}

double Deterministic_Vol_NM_Stein::AuxiliaryExplicit(OptionBase & opt)
{  
	if(!HasExplicit(opt)) 
	{
		throw std::runtime_error(GetName() + ":: has no cv in the choosen option");
	}
	
	return opt.AuxiliaryExplicit(*this);
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	CreatableBase stuff
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

std::string Deterministic_Vol_NM_Stein::GetName() const 
							{ return ClassName<Deterministic_Vol_NM_Stein>(); }
std::string Deterministic_Vol_NM_Stein::GetID() const 
							{ return ClassID<Deterministic_Vol_NM_Stein>(); }
std::string Deterministic_Vol_NM_Stein::GetBaseClassName() const 
						{ return BaseClassName<Deterministic_Vol_NM_Stein>(); }
std::string Deterministic_Vol_NM_Stein::GetStringify() const
{
	std::string nm = GetBaseClassName() + " is " + GetName() + '\n';
	nm += "  Alpha =   " + ut::ConvertToString<double>(alpha_) + '\n';
	nm += "  Beta = " + ut::ConvertToString<double>(beta_) + '\n';
	nm += "  Y0 =  " + ut::ConvertToString<double>(Y0_) + '\n';
	return nm;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	end of file
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
