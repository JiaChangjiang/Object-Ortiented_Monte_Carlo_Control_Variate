//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	Deterministic_Vol_MI_Heston.cpp
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

#include "Deterministic_Vol_MI_Heston.h"

#include "OptionBase.h"

#include "IO_Manager.h"
#include "AppFactoryRegistration.h"
#include "CreateableBase.h"

#include "ParameterY0.h"
#include "ParameterT.h"
#include "ParameterK.h"
#include "ParameterN.h"
#include "ParameterTheta.h"

#include "utility.h"
#include "rv_library.h"

#include <cmath>
#include <string>
#include <iostream>

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  createable
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

template<> std::string ClassName<Deterministic_Vol_MI_Heston>()
{
	return "Deterministic_Vol_MI_Heston";
}

template<> std::string ClassID<Deterministic_Vol_MI_Heston>()
{
	return "MIHeston";
}

template<> std::string BaseClassName<Deterministic_Vol_MI_Heston>()
{
	return ClassName<DeterministicVolatilityBase>();
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  register with factory
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

namespace
{
	AppFactoryRegistration<DeterministicVolatilityBase, Deterministic_Vol_MI_Heston> 
												RegisterDeterministic_Vol_MI_Heston;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	constructor
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

Deterministic_Vol_MI_Heston::Deterministic_Vol_MI_Heston()
:	 N_(IO_Manager::Instance().GetValue<double>(ParameterN()))
,	 T_(IO_Manager::Instance().GetValue<double>(ParameterT()))
,	 k_(IO_Manager::Instance().GetValue<double>(ParameterK()))
,	 theta_(IO_Manager::Instance().GetValue<double>(ParameterTheta()))
,	 Y0_(IO_Manager::Instance().GetValue<double>(ParameterY0()))
{
	dt_ = T_ / N_;
	
	V_.resize(N_ + 1);
	V_[0] = Y0_;
	
	// compute the deterministic volatility at each time step and save as a vector
	for (long i = 1; i <= N_; ++i)
	{
		V_[i] = std::exp(-k_ * dt_ * i) * Y0_ + theta_ * (1 - exp(-k_ * dt_ * i));
	}
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	get next V
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

double Deterministic_Vol_MI_Heston::Next_V(double i)
{
	return V_[ i ];
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	get explicit
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

bool Deterministic_Vol_MI_Heston::HasExplicit(OptionBase & opt)
{
	return opt.HasExplicit(*this);
}

double Deterministic_Vol_MI_Heston::AuxiliaryExplicit(OptionBase & opt)
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

std::string Deterministic_Vol_MI_Heston::GetName() const 
							{ return ClassName<Deterministic_Vol_MI_Heston>(); }
std::string Deterministic_Vol_MI_Heston::GetID() const 
							{ return ClassID<Deterministic_Vol_MI_Heston>(); }
std::string Deterministic_Vol_MI_Heston::GetBaseClassName() const 
						{ return BaseClassName<Deterministic_Vol_MI_Heston>(); }
std::string Deterministic_Vol_MI_Heston::GetStringify() const
{
	std::string nm = GetBaseClassName() + " is " + GetName() + '\n';
	nm += "  Theta =   " + ut::ConvertToString<double>(theta_) + '\n';
	nm += "  K = " + ut::ConvertToString<double>(k_) + '\n';
	nm += "  Y0 =  " + ut::ConvertToString<double>(Y0_) + '\n';
	return nm;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	end of file
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
