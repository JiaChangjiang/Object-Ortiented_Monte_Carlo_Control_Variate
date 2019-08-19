//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
// MonteCarloManager.cpp
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

#include "MonteCarloManager.h"
#include "AppFactory.h"
#include "AppFactoryRegistration.h"

#include "IO_Manager.h"

#include "OptionBase.h"
#include "StochasticVolatilityBase.h"
#include "DeterministicVolatilityBase.h"
#include "ProcessGBM.h"
#include "RandomNumberManager.h"
#include "MonteCarloBase.h"

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX 
//	constructor
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

MonteCarloManager::MonteCarloManager(ProcessGBM & gbm, OptionBase & opt)
{
	gbm_ = &gbm;
	opt_ = &opt;	
	monte_carlo_ = AppFactory<MonteCarloBase>::Instance().CreateObject();
	ran_ = new RandomNumberManager();
}

MonteCarloManager::~MonteCarloManager()
{
	delete monte_carlo_;
	delete ran_;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	run
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

void MonteCarloManager::run()
{
	monte_carlo_->initialise(*gbm_, *opt_,*ran_);

	monte_carlo_->run();
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	set up output
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

void MonteCarloManager::SetOutput(OutputManager& out)
{
	out_ = &out;

	monte_carlo_->SetOutput(*out_);
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	end
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
