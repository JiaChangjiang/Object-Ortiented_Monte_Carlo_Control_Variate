//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  OptionEuroPut.h
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
#ifndef EuroPutH
#define EuroPutH
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

#include <vector>

#include "OptionBase.h"

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  class OptionEuroPut
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

class OptionEuroPut : public OptionBase
{
	public:
		OptionEuroPut();
		~OptionEuroPut();
	
		double ComputePO(double S) const;
		double ComputePayoff(std::vector<double> & path) const;
	    double ComputePayoff(std::vector<double> & path, Control_Variate&) const;
	    
	    //XXXXXXXXX Double dispatch stuff XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
	    bool  HasExplicit(Deterministic_Vol_NM_Heston&) { return true; };
		bool  HasExplicit(Deterministic_Vol_MI_Heston&) { return true; };
		bool  HasExplicit(Deterministic_Vol_MII_Heston&) { return true; };
	
		bool  HasExplicit(Deterministic_Vol_NM_HW&) { return true; };
		bool  HasExplicit(Deterministic_Vol_MI_HW&) { return true; };
		bool  HasExplicit(Deterministic_Vol_MII_HW&) { return true; };
	
		bool  HasExplicit(Deterministic_Vol_NM_Stein&) { return true; };
	
		double  AuxiliaryExplicit(Deterministic_Vol_NM_Heston &);
		double  AuxiliaryExplicit(Deterministic_Vol_MI_Heston &);
		double  AuxiliaryExplicit(Deterministic_Vol_MII_Heston &);
	
		double  AuxiliaryExplicit(Deterministic_Vol_NM_HW &);
		double  AuxiliaryExplicit(Deterministic_Vol_MI_HW &);
		double  AuxiliaryExplicit(Deterministic_Vol_MII_HW &);
	
		double  AuxiliaryExplicit(Deterministic_Vol_NM_Stein &);
		//XXXXXXXXX end of double dispatch stuff XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
	
		// CreatableBase stuff
		std::string GetName() const;
		std::string GetID() const;
		std::string GetBaseClassName() const;
		std::string GetStringify() const;
	
	private:
		double X_;
		double T_;
};

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
#endif
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  End
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
