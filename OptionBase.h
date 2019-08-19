//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  OptionBase.h
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
#ifndef OptionBaseH
#define OptionBaseH
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

#include "CreateableBase.h"

#include <vector>

class Deterministic_Vol_MI_Heston;
class Deterministic_Vol_MI_HW;

class Deterministic_Vol_MII_Heston;
class Deterministic_Vol_MII_HW;

class Deterministic_Vol_NM_Heston;
class Deterministic_Vol_NM_HW;
class Deterministic_Vol_NM_Stein;

class Control_Variate;

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  class OptionBase
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

class OptionBase : public CreateableBase
{
	public:
		virtual ~OptionBase() {}
		
		//XXXXXXXXX Double dispatch stuff XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
		virtual bool  HasExplicit(Deterministic_Vol_NM_Heston&) { return false; };
		virtual bool  HasExplicit(Deterministic_Vol_MI_Heston&) { return false; };
		virtual bool  HasExplicit(Deterministic_Vol_MII_Heston&) { return false; };
	
		virtual bool  HasExplicit(Deterministic_Vol_NM_HW&) { return false; };
		virtual bool  HasExplicit(Deterministic_Vol_MI_HW&) { return false; };
		virtual bool  HasExplicit(Deterministic_Vol_MII_HW&) { return false; };
	
		virtual bool  HasExplicit(Deterministic_Vol_NM_Stein&) { return false; };
	
		virtual double  AuxiliaryExplicit(Deterministic_Vol_NM_Heston&) { return -1000.0; };
		virtual double  AuxiliaryExplicit(Deterministic_Vol_MI_Heston&) { return -1000.0; };
		virtual double  AuxiliaryExplicit(Deterministic_Vol_MII_Heston&) { return -1000.0; };
	
		virtual double  AuxiliaryExplicit(Deterministic_Vol_NM_HW&) { return -1000.0; };
		virtual double  AuxiliaryExplicit(Deterministic_Vol_MI_HW&) { return -1000.0; };
		virtual double  AuxiliaryExplicit(Deterministic_Vol_MII_HW&) { return -1000.0; };
	
		virtual double  AuxiliaryExplicit(Deterministic_Vol_NM_Stein&) { return -1000.0; };
		//XXXXXXXXX end of double dispatch stuff XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
		
		virtual double ComputePO(double S) const = 0;
		virtual double ComputePayoff(std::vector<double>& path) const = 0;
		virtual double ComputePayoff(std::vector<double>& path, Control_Variate&) const = 0;
};

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
#endif
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  End
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
