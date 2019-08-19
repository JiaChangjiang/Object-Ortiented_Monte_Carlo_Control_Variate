//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  GBM_Process.h
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
#ifndef Heston_processH
#define Heston_processH
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

#include "ProcessBase.h"

#include "MethodsBase.h"
#include "ProcessBase.h"

#include <string>
#include <vector>

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  class Heston_Process
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

class Heston_Process : public ProcessBase
{
public:
	Heston_Process();
	~Heston_Process();

	//original process
	double Next_V(double V, long i, double dw1);

	double Next_S(double S, long i, double dw1, double Present_V);

	void FillUpPath(std::vector<double>& path, std::vector<double>& random_number);

	//auxiliary process
	void FillUpPath(std::vector<double>& path, std::vector<double>& dW1, MethodsBase& methods_base);

	double Next_V(long i, MethodsBase& method);

	// CreatableBase stuff
	std::string GetName() const;
	std::string GetID() const;
	std::string GetBaseClassName() const;
	std::string GetStringify() const;

private:
	double S_0_;
	//double sig_;
	double r_;
	double V_0_;

	double heston_rho_;
	double heston_k_;
	double heston_theta_;
	double heston_sigma_;

	double dt_;
};

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
#endif
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  End
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
