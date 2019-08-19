//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	Deterministic_Vol_NM_HW.cpp
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

#include "Deterministic_Vol_NM_HW.h"

#include "OptionBase.h"

#include "IO_Manager.h"
#include "AppFactoryRegistration.h"
#include "CreateableBase.h"

#include "ParameterY0.h"
#include "ParameterHWSigma.h"
#include "ParameterMiu.h"
#include "ParameterHWM.h"
#include "ParameterT.h"
#include "ParameterN.h"

#include "utility.h"
#include "rv_library.h"

#include <cmath>
#include <string>
#include <iostream>

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  createable
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

template<> std::string ClassName<Deterministic_Vol_NM_HW>()
{
	return "Deterministic_Vol_NM_HW";
}

template<> std::string ClassID<Deterministic_Vol_NM_HW>()
{
	return "NMHW";
}

template<> std::string BaseClassName<Deterministic_Vol_NM_HW>()
{
	return ClassName<DeterministicVolatilityBase>();
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  register with factory
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

namespace
{
	AppFactoryRegistration<DeterministicVolatilityBase, Deterministic_Vol_NM_HW> 
												RegisterDeterministic_Vol_NM_HW;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	constructor
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

Deterministic_Vol_NM_HW::Deterministic_Vol_NM_HW()
:	 N_(IO_Manager::Instance().GetValue<long>(ParameterN()))
,	 T_(IO_Manager::Instance().GetValue<double>(ParameterT()))
,	 m_(IO_Manager::Instance().GetValue<double>(ParameterHWM()))
,	 miu_(IO_Manager::Instance().GetValue<double>(ParameterMiu()))
,	 sig_(IO_Manager::Instance().GetValue<double>(ParameterHWSigma()))
,	 Y0_(IO_Manager::Instance().GetValue<double>(ParameterY0()))	
{
	dt_ = T_ / N_;
	
	V_.resize(N_ + 1);
	V_[0] = Y0_;

	// compute the deterministic volatility at each time step and save as a vector
	for (long i = 1; i <= N_; ++i)
	{
		double sig = std::sqrt(Y0_) * exp(0.5 * dt_ * i 
										* (miu_ + 0.5 * (m_ - 1) * sig_ * sig_));

		V_[i] = sig * sig;
	}
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	get next V
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

double Deterministic_Vol_NM_HW::Next_V(double i)
{
	return V_[ i ];
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	get explicit
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

bool Deterministic_Vol_NM_HW::HasExplicit(OptionBase & opt)
{
	return opt.HasExplicit(*this);
}

double Deterministic_Vol_NM_HW::AuxiliaryExplicit(OptionBase & opt)
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

std::string Deterministic_Vol_NM_HW::GetName() const 
								{ return ClassName<Deterministic_Vol_NM_HW>(); }
std::string Deterministic_Vol_NM_HW::GetID() const 
								{ return ClassID<Deterministic_Vol_NM_HW>(); }
std::string Deterministic_Vol_NM_HW::GetBaseClassName() const 
							{ return BaseClassName<Deterministic_Vol_NM_HW>(); }
std::string Deterministic_Vol_NM_HW::GetStringify() const
{
	std::string nm = GetBaseClassName() + " is " + GetName() + '\n';
	nm += "  sig =   " + ut::ConvertToString<double>(sig_) + '\n';
	nm += "  Miu = " + ut::ConvertToString<double>(miu_) + '\n';
	nm += "  Y0 =  " + ut::ConvertToString<double>(Y0_) + '\n';
	return nm;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	end of file
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
