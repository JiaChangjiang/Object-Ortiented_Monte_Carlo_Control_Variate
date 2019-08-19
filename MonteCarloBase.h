//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  MonteCarloBase.h
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
#ifndef MonteCarloBaseH
#define MonteCarloBaseH
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

#include<vector>
#include"CreateableBase.h"

class CreateableBase;
class OutputManager;
class OptionBase;
class ProcessGBM;
class StochasticVolatilityBase;
class DeterministicVolatilityBase;
class RandomNumberManager;
class StopWatch;

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  class MonteCarloBase
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

class MonteCarloBase: public CreateableBase
{
	public:
		virtual ~MonteCarloBase() {};
		
		virtual void initialise(ProcessGBM & gbm, OptionBase & opt, RandomNumberManager & ran)=0;
		
		virtual void run() = 0;
		
		virtual double GetOptionValue() const = 0;
		virtual double GetOptionSE() const = 0;
		virtual double GetMCTime() const = 0;
		
		virtual void SetOutput(OutputManager & out) = 0;
};

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
#endif
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  End
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
