//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	MonteCarlo_CV.cpp
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

#include <vector>
#include <string>

#include "MonteCarlo_CV.h"

#include "Accumulator.h"
#include "Accumulator_CV.h"

#include "OptionBase.h"
#include "ProcessGBM.h"
#include "DeterministicVolatilityBase.h"
#include "StochasticVolatilityBase.h"

#include "MonteCarloBase.h"
#include "MonteCarlo_Plain.h"
#include "RandomNumberManager.h"

#include "Control_Variate.h"

#include "AppFactory.h"
#include "AppFactoryRegistration.h"
#include "IO_Manager.h"
#include "OutputManager.h"
#include "MonitorManager.h"
#include "ConfigurationManager.h"

#include"AnalyticalFormule.h"

#include "utility.h"

#include "ParameterN.h"
#include "ParameterM.h"

#include "StopWatch.h"

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  createable
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

template<> std::string ClassName<MonteCarlo_CV>()
{
	return "MonteCarlo_CV";
}

template<> std::string ClassID<MonteCarlo_CV>()
{
	return "cv";
}

template<> std::string BaseClassName<MonteCarlo_CV>()
{
	return ClassName<MonteCarloBase>();
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  register with factory
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

namespace
{
	AppFactoryRegistration<MonteCarloBase, MonteCarlo_CV> RegisterMonteCarlo_CV;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	constructor
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

MonteCarlo_CV::MonteCarlo_CV()
:	 M_(IO_Manager::Instance().GetValue<long>(ParameterM()))
,	 N_(IO_Manager::Instance().GetValue<long>(ParameterN()))
{
	mon_ = ConfigurationManager::Instance().GetMonitor();
	deterministic_ = AppFactory<DeterministicVolatilityBase>::Instance().CreateObject();
	plain_mc_ = new MonteCarlo_Plain();
	aux_acc_ = new Accumulator();
	cv_acc_ = new Accumulator_CV();
	stw_MC_CV_ = new StopWatch;
	aux_acc_ = new Accumulator();
	cv_method_ = new Control_Variate(*this);
}

MonteCarlo_CV::~MonteCarlo_CV()
{
	delete plain_mc_;
	delete aux_acc_;
	delete cv_method_;	
	delete cv_acc_;
	delete stw_MC_CV_;
	delete deterministic_;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	Initialize
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

void MonteCarlo_CV::initialise(ProcessGBM & gbm,  OptionBase & opt, RandomNumberManager &ran)
{
	gbm_ = & gbm;
	opt_ = & opt;
	ran_ = & ran;
	gbm_->Initialise(*deterministic_);
	plain_mc_->initialise(*gbm_, *opt_,*ran_);
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	run Monte Carlo
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

double MonteCarlo_CV::run_one_path(double j) 
{
	std::vector<double>& random_number = ran_->GetRandomNumber();

	std::vector<double> path(N_ + 1);

	gbm_->FillUpPath(path, random_number, *deterministic_);

	double Auxiliary_payoff = opt_->ComputePayoff(path,*cv_method_);

	aux_acc_->AddValue(Auxiliary_payoff);

	return Auxiliary_payoff;
}
 
void MonteCarlo_CV::run_paths() 
{
	mon_->OutputBanner("MC_method with auxiliary process is running");

	stw_MC_CV_->StartStopWatch();

	for (long j = 1; j <= M_; ++j)
	{
		ran_->UpdateRandomNumber(); //update the random n

		double payoff = plain_mc_->run_one_path(j);

		cv_acc_->AddPlainPrice(payoff);

		double auxiliary_payoff = run_one_path(j); 

		double cv = cv_method_->Compute_CV(auxiliary_payoff, auxiliary_explicit_);

		cv_acc_->AddCV(cv);	
	}

	stw_MC_CV_->StopStopWatch();
}

void MonteCarlo_CV::run()
{
	ComputePlainMCTime();

	RunAuxiliaryAnalytic();

	run_paths();
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	Run Auxiliary Analytic
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

void MonteCarlo_CV::RunAuxiliaryAnalytic()
{
	auxiliary_explicit_ = deterministic_->AuxiliaryExplicit(*opt_);	
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	compute the running time of Monte Carlo Plain
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

void MonteCarlo_CV::ComputePlainMCTime() 
{
	plain_mc_->run();
	plain_mc_->clear();
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	Set Output
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

void MonteCarlo_CV::SetOutput(OutputManager & out)
{
	out_ = & out;

	double plain_mc_se = plain_mc_->GetOptionSE();
	double plain_mc_time = plain_mc_->GetMCTime(); // get running time for Monte Carlo Plain

	double cv_mc_se = cv_acc_->GetSE();
	double cv_mc_time = GetMCTime(); // get running time for Monte Carlo CV
	plain_mc_->SetOutput(*out_);

	out_->RegisterOutput("Auxiliary Process	                ", auxiliary_explicit_);
	out_->RegisterOutput("CV Option Value	                    	", GetOptionValue());
	out_->RegisterOutput("CV Option SE				", cv_mc_se); 
	out_->RegisterOutput("CV Time taken				", cv_mc_time);

	out_->RegisterOutput("Variance Reduction Ratio		",
		( plain_mc_time * (plain_mc_se * plain_mc_se) ) / ( cv_mc_time * (cv_mc_se * cv_mc_se) ) );
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	getters
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

double MonteCarlo_CV::GetOptionSE() const
{
	return cv_acc_->GetSE();
}

double MonteCarlo_CV::GetMCTime() const
{
	return stw_MC_CV_->GetTime();
}

double MonteCarlo_CV::GetOptionValue() const
{
	return cv_acc_->GetValue();
}
                           
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	CreatableBase stuff
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

std::string MonteCarlo_CV::GetName() const { return ClassName<MonteCarlo_CV>(); }
std::string MonteCarlo_CV::GetID() const { return ClassID<MonteCarlo_CV>(); }
std::string MonteCarlo_CV::GetBaseClassName() const { return BaseClassName<MonteCarlo_CV>(); }
std::string MonteCarlo_CV::GetStringify() const
{
	std::string nm = GetBaseClassName() + " is " + GetName() + '\n';
	return nm;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	end
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
