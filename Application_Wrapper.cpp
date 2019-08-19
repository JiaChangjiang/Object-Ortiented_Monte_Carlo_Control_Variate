//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	Application_Wrapper.cpp
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

#include "Application_Wrapper.h"

#include "ConfigurationManager.h"
#include "MonitorManager.h"

#include "IO_Manager.h"
#include "OutputManager.h"
#include "StopWatch.h"

#include"Valuation.h"

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	constructor
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

Application_Wrapper::Application_Wrapper()
{
	mon_ = ConfigurationManager::Instance().GetMonitor();
	out_ = IO_Manager::Instance().GetOutputManager();

    stw_ = new StopWatch;
    val_ = new Valuation();
}
                                          
Application_Wrapper::~Application_Wrapper()
{
   delete stw_;
   delete val_;
}
                                           
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	run
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

void Application_Wrapper::run()
{
	mon_->OutputBanner("Monte Carlo version 30:  Has a wrapper object to own IO");

	out_->Clear();
    
	stw_->StartStopWatch();

	val_->run( );

	val_->SetOutput(*out_);
        
	out_->RegisterOutput("Time taken				", stw_->GetTime());

    out_->DoOutput();

	mon_->OutputBanner("Finished");
}
 
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	end
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
