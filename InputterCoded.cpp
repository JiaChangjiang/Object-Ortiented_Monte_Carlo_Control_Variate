//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	InputterCoded.cpp
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

#include "InputterCoded.h"
#include "MonitorManager.h"

#include "IOFactoryRegistration.h"
#include "CreateableBase.h"

#include "ParameterBase.h"

#include "ParameterS0.h"
#include "ParameterX.h"
#include "ParameterR.h"
#include "ParameterSigma.h"
#include "ParameterRho.h"

#include "ParameterM.h"
#include "ParameterN.h"
#include "ParameterT.h"

#include "ParameterY0.h"

#include "ParameterHWSigma.h"
#include "ParameterHWM.h"
#include "ParameterMiu.h"

#include "ParameterK.h"
#include "ParameterTheta.h"

#include "ParameterAlpha.h"
#include "ParameterBeta.h"


#include <map>
#include <string>

class DeterministicVolatilityBase;
class StochasticVolatilityBase;
class OptionBase;
class MonteCarloBase;

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  createable
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

template<> std::string ClassName<InputterCoded>()
{
    return "InputterCoded";
}

template<> std::string ClassID<InputterCoded>()
{
    return "c";
}

template<> std::string BaseClassName<InputterCoded>()
{
    return ClassName<InputterBase>();
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  register with factory
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

namespace
{
	IOFactoryRegistration<InputterBase, InputterCoded> RegisterInputterCoded;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	interface
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

InputterCoded::InputterCoded()
{
	mon_->OutputLine("Input is hard wired");

    sd_map_.clear();
   	sd_map_[ParameterS0().name] = 0;
	sd_map_[ParameterX().name] = 0;
   	sd_map_[ParameterR().name] = 0;
   	sd_map_[ParameterSigma().name] = 0;
	sd_map_[ParameterRho().name] = 0;

   	sd_map_[ParameterM().name] = 0;
   	sd_map_[ParameterN().name] = 0;
   	sd_map_[ParameterT().name] = 0;

	sd_map_[ParameterY0().name] = 0;
   	
   	// parameters of Hull-White
   	sd_map_[ParameterHWSigma().name] = 0; // Sigma for Hull-White
   	sd_map_[ParameterHWM().name] = 0; // Order of Hull-White
   	sd_map_[ParameterMiu().name] = 0;
	
	// parameters of Heston
	sd_map_[ParameterK().name] = 0;
	sd_map_[ParameterTheta().name] = 0;
	
	// parameters of Stein Stein
	sd_map_[ParameterAlpha().name] = 0;
	sd_map_[ParameterBeta().name] = 0;

   	ss_map_.clear();
	ss_map_[ClassName<DeterministicVolatilityBase>()] = "NMHW";
	// NMHW is new method for the HuLL-White model,
	// NMHeston is the new method for Heston model,
	// NMStein is the new method for Stein-Stein model
	// MIHW is method I for the Hull-White model
	// MIHeston is the method I for Heston model
	// MIStein is the method I for Stein-Stein model
	// MIIHW is the method II for Hull-White model
	// MIIHeston is the method II for the Heston model
	ss_map_[ClassName<StochasticVolatilityBase>()] = "HW";
	//HW for Hull-White,Heston for Heston,Stein for Stein-Stein
	ss_map_[ClassName<OptionBase>()] = "p";
	// p for Europut option,c for Eurocall option,
	// asian_ari_c for Arithmetric Asian call option
	// asian_ari_p for Arithmetric Asian put option
	// asian_geo_c for Geometric Asian call option
	// asian_geo_p for Geometric Asian put option
	ss_map_[ClassName<MonteCarloBase>()] = "cv";	
	//plain for Plain Monte Carlo
	//cv for Control Variate Monte Carlop
	
}
  
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	end
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
