//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  OptionCtsGeometricAveragePut.h
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
#ifndef OptionCtsGeometricAveragePutH
#define OptionCtsGeometricAveragePutH
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

#include "OptionBase.h"
#include "Control_Variate.h"

#include <vector>

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  class OptionCtsGeometricAveragePut
//  This is actually a discrete Geo av. call with reset interval dt.
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

class OptionCtsGeometricAveragePut : public OptionBase
{
	public:
	    OptionCtsGeometricAveragePut();
		OptionCtsGeometricAveragePut(double X, double T);
		~OptionCtsGeometricAveragePut() {};
	
		double ComputePO (double S) const;
		double ComputePayoff (std::vector<double>& path) const;
		double ComputePayoff (std::vector<double>& path, Control_Variate&) const;
		
		//XXXXXXXXX Double dispatch stuff XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
		bool  HasExplicit(Deterministic_Vol_NM_Heston&) { return false; };
		bool  HasExplicit(Deterministic_Vol_MI_Heston&) { return false; };
		bool  HasExplicit(Deterministic_Vol_MII_Heston&) { return false; };
	
		bool  HasExplicit(Deterministic_Vol_NM_HW&) { return true; };
		bool  HasExplicit(Deterministic_Vol_MI_HW&) { return false; };
		bool  HasExplicit(Deterministic_Vol_MII_HW&) { return false; };
	
		bool  HasExplicit(Deterministic_Vol_NM_Stein&) { return false; };
		
		double  AuxiliaryExplicit(Deterministic_Vol_NM_HW&) { return -1000; };
		double  AuxiliaryExplicit(Deterministic_Vol_NM_Heston&) { return -1000; };
		double  AuxiliaryExplicit(Deterministic_Vol_MI_Heston&) { return -1000; };
		double  AuxiliaryExplicit(Deterministic_Vol_MII_Heston&) { return -1000; };
	
		double  AuxiliaryExplicit(Deterministic_Vol_MI_HW&) { return -1000; };
		double  AuxiliaryExplicit(Deterministic_Vol_MII_HW&) { return -1000; };
	
		double  AuxiliaryExplicit(Deterministic_Vol_NM_Stein&) { return -1000; };
		//XXXXXXXXX end of double dispatch stuff XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
	
		// CreatableBase stuff
		std::string GetName() const;
		std::string GetID() const;
		std::string GetBaseClassName() const;
		std::string GetStringify() const;
	
	private:
		double X_;
		double T_;
	
		double AverageGeometricFirstExcluded(std::vector<double>& path) const;
};

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
#endif
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  End
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
