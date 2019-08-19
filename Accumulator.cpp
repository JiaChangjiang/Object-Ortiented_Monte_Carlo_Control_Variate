//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	Accumulator.cpp
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

#include "Accumulator.h"

#include "IO_Manager.h"

#include "ParameterT.h"
#include "ParameterR.h"

#include <cmath>

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	constructor
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

Accumulator::Accumulator()
:	acc_vals_(0.0)
,	acc_squs_(0.0)
,	M_(0)
{
    discount_ = std::exp(-IO_Manager::Instance().GetValue<double>(ParameterR()) 
						* IO_Manager::Instance().GetValue<double>(ParameterT()));
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	accumulate
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

void Accumulator::AddValue(double payoff)
{
    acc_vals_ += payoff;
    acc_squs_ += payoff * payoff;
    ++M_;
}
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	clear
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

void Accumulator::clear()
{
	acc_vals_ = 0.0;
	acc_squs_ = 0.0;
	M_ = 0;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	getter
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

double Accumulator::GetValue() const
{
    return discount_ * acc_vals_ / M_;
}

double Accumulator::GetSE() const
{
    return discount_ * std::sqrt(acc_squs_ - acc_vals_ * acc_vals_ / M_) / M_;
}

long Accumulator::GetM() const
{
    return M_;
}
                
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	end
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
