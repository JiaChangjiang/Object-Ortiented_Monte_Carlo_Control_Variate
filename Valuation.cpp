//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	Valuation.cpp
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

#include "Valuation.h"

#include"MonteCarloBase.h"

#include "AnalyticalFormule.h"
#include "Control_Variate.h"

#include "OptionBase.h"
#include "StochasticVolatilityBase.h"
#include "DeterministicVolatilityBase.h"
#include "ProcessGBM.h"

#include "AppFactory.h"
#include "AppFactoryRegistration.h"

#include "IO_Manager.h"
#include "OutputManager.h"
#include "MonteCarloManager.h"

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	constructor
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

Valuation::Valuation()
{
    opt_ = AppFactory<OptionBase>::Instance().CreateObject();
	gbm_ = new ProcessGBM();
	stochastic_ = AppFactory<StochasticVolatilityBase>::Instance().CreateObject();
	monte_carlo_manager_ = new MonteCarloManager(*gbm_, *opt_);
}

Valuation::~Valuation()
{
    delete opt_;
	delete gbm_;
    delete monte_carlo_manager_;
	delete stochastic_;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	run
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

void Valuation::run()
{
	gbm_->Initialise(*stochastic_);

	monte_carlo_manager_->run();
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	SetOutput
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

void Valuation::SetOutput(OutputManager & out)
{
	out_ = & out;
	
	monte_carlo_manager_->SetOutput(*out_);
}
 
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	end
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
