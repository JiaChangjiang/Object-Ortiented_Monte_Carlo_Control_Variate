//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  MonteCarlo_CV.h
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
#ifndef MonteCarlo_CVH
#define MonteCarlo_CVH
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

#include<vector>

#include"MonteCarloBase.h"

class OutputManager;
class MonitorManager;
class Accumulator;
class Accumulator_CV; 
class MC_Path;
class Auxiliary_Path;
class Random;
class OptionBase;
class StochasticVolatilityBaseBase;
class DeterministicVolatilityBase;
class ProcessGBM;
class MonteCarloBase;
class MonteCarlo_Plain;
class Control_Variate;
class RandomNumberManager;
class StopWatch;

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  class MC_method
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

class MonteCarlo_CV: public MonteCarloBase
{
	public:
		MonteCarlo_CV();  
		~MonteCarlo_CV();
	
		void initialise(ProcessGBM& gbm, OptionBase& opt, RandomNumberManager& ran);
		
		double run_one_path(double j);
		
		void run();
	
		void SetOutput(OutputManager & out);
		
		// CreatableBase stuff
		std::string GetName() const;
		std::string GetID() const;
		std::string GetBaseClassName() const;
		std::string GetStringify() const;
	
	private:
		long M_;
		long N_;
	
		double auxiliary_explicit_;
	
		MonitorManager * mon_;
		OutputManager * out_;
		ProcessGBM * gbm_;
		DeterministicVolatilityBase * deterministic_;
		OptionBase * opt_;
		MonteCarlo_Plain * plain_mc_;
		Accumulator * aux_acc_;
		Accumulator_CV * cv_acc_;
		Control_Variate * cv_method_;
		RandomNumberManager * ran_;
		StopWatch * stw_MC_CV_;

		void ComputePlainMCTime();
		
		void run_paths();	
		
		void RunAuxiliaryAnalytic();
		
		double GetOptionValue() const;

		double GetOptionSE() const;

		double GetMCTime() const;

};

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
#endif
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  End
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

