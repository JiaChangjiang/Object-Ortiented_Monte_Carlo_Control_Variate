//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  OptionCtsAveragePut.h
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
#ifndef OptionCtsAveragePutH
#define OptionCtsAveragePutH
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

#include <vector>

#include "OptionBase.h"

class Control_Variate;

class OptionCtsGeometricAveragePut;

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  class OptionCtsAveragePut
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

class OptionCtsAveragePut : public OptionBase
{
	public:	
		OptionCtsAveragePut();
		~OptionCtsAveragePut();
	
		double ComputePO(double S) const;
	
		double ComputePayoff(std::vector<double> & path) const;

		double ComputePayoff(std::vector<double> & path, Control_Variate &) const;
		
		//XXXXXXXXX Double dispatch stuff XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
		bool  HasExplicit(Deterministic_Vol_NM_Heston&) { return false; };
		bool  HasExplicit(Deterministic_Vol_MI_Heston&) { return false; };
		bool  HasExplicit(Deterministic_Vol_MII_Heston&) { return false; };
	
		bool  HasExplicit(Deterministic_Vol_NM_HW&) { return false; };
		bool  HasExplicit(Deterministic_Vol_MI_HW&) { return false; };
		bool  HasExplicit(Deterministic_Vol_MII_HW&) { return false; };
	
		bool  HasExplicit(Deterministic_Vol_NM_Stein&) { return false; };
		
		double  AuxiliaryExplicit(Deterministic_Vol_NM_HW &) { return -1000; }
		double  AuxiliaryExplicit(Deterministic_Vol_NM_Heston &) { return -1000; }
		double  AuxiliaryExplicit(Deterministic_Vol_MI_Heston &) { return -1000; }
		double  AuxiliaryExplicit(Deterministic_Vol_MII_Heston &) { return -1000; }

		double  AuxiliaryExplicit(Deterministic_Vol_MI_HW &) { return -1000; }
		double  AuxiliaryExplicit(Deterministic_Vol_MII_HW &) { return -1000; }

		double  AuxiliaryExplicit(Deterministic_Vol_NM_Stein &) { return -1000; }
		//XXXXXXXXX end of double dispatch stuff XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
	
		// CreatableBase stuff
		std::string GetName() const;
		std::string GetID() const;
		std::string GetBaseClassName() const;
		std::string GetStringify() const;
			
	private:
		double X_;
		double T_;
		
		OptionCtsGeometricAveragePut* Geo_asian_;
		
		double AverageArithmeticFirstExcluded(std::vector<double>& path) const;
};

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
#endif
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  End
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
