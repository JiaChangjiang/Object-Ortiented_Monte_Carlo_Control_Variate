//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	Input.cpp
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

#include "Input.h"

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	interface
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

Input::Input()
{
	M_ = 3000;
	N_ = 100;
	X_ = 100;
	T_ = 0.5;
	r_ = 0.1;

	//hull_white
	hullwhite_S_0_ = 40;
	hullwhite_r_ = 0.05;
	hullwhite_X_ = 40;
	hullwhite_T_ = 0.5;
	hullwhite_m_ = 1;

	hullwhite_miu_ = 0.02;
	hullwhite_sig_ = 0.1;
	hullwhite_rho_ = 0;
	hullwhite_Y0_ = 0.02;

	change_hullwhite_S_0_ = { 34,36,40,44,46,50 };
	change_hullwhite_rho_ = { -1,-0.6,-0.1,0,0.1,0.6,1 };
	change_hullwhite_m_ = { -75,-50,-10,0,1,2,5,10,75 };
	//heston
	heston_S_0_ = 100;
	heston_r_ = 0;
	heston_X_ = 100;
	heston_T_ = 0.5;
	heston_m_ = 1;

	heston_k_ = 2;
	heston_theta_ = 0.01;
	heston_sig_ = 0.1;
	heston_rho_ = 0;
	heston_Y0_ = 0.01;

	change_heston_S_0_ = { 90,100,110 };
	change_heston_rho_ = { -1,-0.6,-0.1,0,0.1,0.6,1 };
	change_heston_sig_ = { 0.01,0.05,0.1,0.15,0.2,0.25 };
	change_heston_T_ = { 0.25,0.5,0.75,1,1.5 };
	change_heston_k_ = { 1,2,4,8,14 };

	//stein_stein
	steinstein_S_0_ = 100;
	steinstein_r_ = 0.095;
	steinstein_X_ = 100;
	steinstein_T_ = 0.5;
	steinstein_m_ = 1;

	steinstein_alpha_ = 4;
	steinstein_beta_ = 0.2;
	steinstein_sig_ = 0.1;
	steinstein_rho_ = 0;
	steinstein_Y0_ = 0.1;

	change_steinstein_rho_ = { -1,-0.6,-0.1,0,0.1,0.6,1 };
	change_steinstein_sig_ = { 0.01,0.1,0.15,0.2 };
	change_steinstein_T_ = { 1 / 12,0.25,0.5,0.75,1 };
	change_steinstein_alpha_ = { 4,8,14,16,20,100 };

	//option type
	std::string optiontype = "c";

	processtype = "Heston_process";
    methodtype = "New_Method";

	std::vector<std::string> Process = { "Heston_process","Hull_White_process","Stein_Stein_process" };
	std::vector<std::string> Methods = { "Method_I","New_Method","Method_II"," Const_Volatility" };
	std::vector<std::string> Parameter = { "hoishd" };
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	end
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
