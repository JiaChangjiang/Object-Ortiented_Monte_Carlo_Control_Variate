//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	MonteCarlo_Plain.cpp
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

#include "MonteCarloBase.h"
#include "MonteCarlo_Plain.h"
#include "Accumulator.h"
#include "IO_Manager.h"
#include "OutputManager.h"
#include "AppFactoryRegistration.h"
#include "MonitorManager.h"
#include "ConfigurationManager.h"
#include "ParameterN.h"
#include "ParameterM.h"
#include "OptionBase.h"
#include "ProcessGBM.h"
#include "DeterministicVolatilityBase.h"
#include "StochasticVolatilityBase.h"
#include"utility.h"
#include "RandomNumberManager.h"
#include "StopWatch.h"

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  createable
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

template<> std::string ClassName<MonteCarlo_Plain>()
{
	return "MonteCarlo_Plain";
}

template<> std::string ClassID<MonteCarlo_Plain>()
{
	return "plain";
}

template<> std::string BaseClassName<MonteCarlo_Plain>()
{
	return ClassName<MonteCarloBase>();
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  register with factory
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

namespace
{
	AppFactoryRegistration<MonteCarloBase, MonteCarlo_Plain> RegisterMonteCarlo_Plain;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  consturctor
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

MonteCarlo_Plain::MonteCarlo_Plain()
:	M_(IO_Manager::Instance().GetValue<long>(ParameterM()))
,	N_(IO_Manager::Instance().GetValue<long>(ParameterN()))	
{
	
	mon_ = ConfigurationManager::Instance().GetMonitor();
	stw_mc_ = new StopWatch;
	acc_ = new Accumulator;
}

MonteCarlo_Plain::~MonteCarlo_Plain()
{
	delete acc_;
	delete stw_mc_;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	Initialise
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

void MonteCarlo_Plain::initialise(ProcessGBM& gbm, OptionBase& opt, RandomNumberManager& ran)
{
	ran_ = & ran;
	gbm_ = & gbm;
	opt_ = & opt;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	run
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

double MonteCarlo_Plain::run_one_path(double j) 
{
	std::vector<double> path(N_ + 1);

	std::vector<double> random_number= ran_->GetRandomNumber();

	mon_->OutputCounter(j, M_, 50000);

	gbm_->FillUpPath(path, random_number);

	double payoff = opt_->ComputePayoff(path);

	acc_->AddValue(payoff);
	
	return payoff;
}

void MonteCarlo_Plain::run()
{
	mon_->OutputBanner("plain Monte-Carlo is running");

	stw_mc_->StartStopWatch();

	ran_->UpdateRandomNumber();

	for (long j = 1; j <= M_; ++j)
	{
		double payoff = run_one_path(j);
	}
	
	stw_mc_->StopStopWatch();
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	Set Output
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

void MonteCarlo_Plain::SetOutput(OutputManager & out)
{
	out_ = & out;
	out_->RegisterOutput("Plain MC OptionValue			", GetOptionValue());
	out_->RegisterOutput("Plain MC OptionSE			", GetOptionSE());
	out_->RegisterOutput("Plain MC Time taken			", GetMCTime());
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	getter 
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

double MonteCarlo_Plain::GetOptionValue() const
{
	return acc_->GetValue();
}

double MonteCarlo_Plain::GetOptionSE() const
{
	return acc_->GetSE();
}

double MonteCarlo_Plain::GetMCTime() const
{
	return stw_mc_->GetTime();
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	clear the accumulator
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

void MonteCarlo_Plain::clear()
{
	acc_->clear();
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	CreatableBase stuff
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

std::string MonteCarlo_Plain::GetName() const { return ClassName<MonteCarlo_Plain>(); }
std::string MonteCarlo_Plain::GetID() const { return ClassID<MonteCarlo_Plain>(); }
std::string MonteCarlo_Plain::GetBaseClassName() const { return BaseClassName<MonteCarlo_Plain>(); }

std::string MonteCarlo_Plain::GetStringify() const
{
	std::string nm = GetBaseClassName() + " is " + GetName() + '\n';
	return nm;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	end
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
