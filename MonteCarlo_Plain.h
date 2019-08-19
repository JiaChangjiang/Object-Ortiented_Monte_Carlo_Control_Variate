//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
// MonteCarlo_Plain.h
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
#ifndef MonteCarlo_PlainH
#define MonteCarlo_PlainH
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

#include<vector>
#include"MonteCarloBase.h"

class OutputManager;
class MonitorManager;
class Accumulator;
class OptionBase;
class ProcessGBM;
class DeterministicVolatilityBase;
class StochasticVolatilityBase;
class MonteCarloBase;
class OutputManager;
class MonitorManager;
class RandomNumberManager;
class StopWatch;

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  class MonteCarlo_Plain
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

class MonteCarlo_Plain: public MonteCarloBase
{
    public:
        MonteCarlo_Plain();
        ~MonteCarlo_Plain();
        
		void initialise(ProcessGBM & gbm, OptionBase & opt, RandomNumberManager & ran);
		
		double run_one_path(double j);

	    void run() ;
	    
	    void SetOutput(OutputManager & out);

		double GetOptionValue() const;
		double GetOptionSE() const;
		double GetMCTime() const;

		void clear();
		
		// CreatableBase stuff
		std::string GetName() const;
		std::string GetID() const;
		std::string GetBaseClassName() const;
		std::string GetStringify() const;

    private:
		long M_;
		long N_;
		
		OutputManager * out_;	
		ProcessGBM * gbm_;
		OptionBase * opt_;
        Accumulator * acc_;
		MonitorManager * mon_;
		RandomNumberManager * ran_;
		StopWatch * stw_mc_;
};

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
#endif
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  End
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
