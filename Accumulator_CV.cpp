//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	Accumulator_CV.cpp
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

#include "Accumulator_CV.h"

#include "IO_Manager.h"
#include "AppFactoryRegistration.h"
#include "CreateableBase.h"

#include "ParameterR.h"
#include "ParameterT.h"

#include "utility.h"
#include "RegressionOLS.h"

#include <cmath>

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  createable
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

template<> std::string ClassName<Accumulator_CV>()
{
	return "Accumulator_CV";
}

template<> std::string ClassID<Accumulator_CV>()
{
	return "acc_cv";
}

template<> std::string BaseClassName<Accumulator_CV>()
{
	return ClassName<AccumulatorBase>();
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  register with factory
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

namespace
{
	AppFactoryRegistration<AccumulatorBase, Accumulator_CV> RegisterAccumulator_CV;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	constructor
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

Accumulator_CV::Accumulator_CV()
: 	 acc_vals_(0.0)
,	 acc_squs_(0.0)
,	 Is_computed_(false)
{
	ols_ = new RegressionOLS;
	vals_.resize(0);
	cv_.resize(0);
	discount_ = std::exp(-IO_Manager::Instance().GetValue<double>(ParameterR()) 
						* IO_Manager::Instance().GetValue<double>(ParameterT()));	
}

Accumulator_CV::~Accumulator_CV()
{
	delete ols_;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	Add value in accumulator
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

void Accumulator_CV::AddPlainPrice(double payoff)
{
	vals_.push_back(payoff * discount_);
}

void Accumulator_CV::AddCV(double cv)
{
	cv_.push_back(cv);
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	getters
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

double Accumulator_CV::GetValue() const
{
	long M = vals_.size();

	Compute();

	return  acc_vals_ / M;
}

double Accumulator_CV::GetSE() const
{
	long M = vals_.size();

	Compute();

	double radix = acc_squs_ - acc_vals_ * acc_vals_ / M;

	return std::sqrt(radix) / M;
}

long Accumulator_CV::GetM() const
{
	return vals_.size();
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	DoPlainAccumulator().  Case with no cvs. Set acc_vals_ and acc_squs_
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

void Accumulator_CV::Compute() const
{
	if (!Is_computed_)
	{
		DoSimpleAccumulator();
	}
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	DoSimpleAccumulator().  Case with 1 cv. Set acc_vals_ and acc_squs_
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

void Accumulator_CV::DoSimpleAccumulator() const
{
	ut::OutputLine("One CV");
	long M = vals_.size();

	double cv_average = 0.0;
	for (long i = 0; i != M; ++i)
	{
		cv_average += cv_[i];
	}
	cv_average /= M;

	double a;   // regression constant coeefficent
	double b;   // regression slope coeefficent
	double rho; // regression R^2    

	ols_->SimpleRegression(a, b, cv_, vals_, rho);

	ut::OutputLine("Belta:       ", b);
	ut::OutputLine("CV average:  ", cv_average);
	ut::OutputLine("Correlation: ", rho);

	for (long i = 0; i != M; ++i)
	{
		double corrected_val = vals_.at(i) - b * cv_.at(i);
		acc_vals_ += corrected_val;
		acc_squs_ += corrected_val * corrected_val;
	}

	Is_computed_ = true;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	CreatableBase stuff
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

std::string Accumulator_CV::GetName() const { return ClassName<Accumulator_CV>(); }
std::string Accumulator_CV::GetID() const { return ClassID<Accumulator_CV>(); }
std::string Accumulator_CV::GetBaseClassName() const { return BaseClassName<Accumulator_CV>(); }
std::string Accumulator_CV::GetStringify() const
{
	std::string nm = GetBaseClassName() + " is " + GetName() + '\n';
	nm += "  Is_computed =          " + ut::ConvertToString<bool>(Is_computed_) + '\n';
	nm += "  accumulated values =   " + ut::ConvertToString<double>(acc_vals_) + '\n';
	nm += "  accumulated squares =  " + ut::ConvertToString<double>(acc_squs_) + '\n';
	return nm;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	end
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
