//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  Control_Variate.cpp
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

#include "Control_Variate.h"

#include "MonteCarlo_CV.h"
#include "AnalyticalFormule.h"

#include "IO_Manager.h"
#include "OutputManager.h"

#include "utility.h"

#include "ParameterM.h"
#include "RegressionOLS.h"

#include <cmath>
#include "ParameterR.h"
#include "ParameterT.h"

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  Constructor
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

Control_Variate::Control_Variate(MonteCarlo_CV& monte_carlo)
:	M_(IO_Manager::Instance().GetValue<long>(ParameterM()))
{
	discount_ = std::exp(-IO_Manager::Instance().GetValue<double>(ParameterR()) 
						* IO_Manager::Instance().GetValue<double>(ParameterT()));
}

Control_Variate::~Control_Variate()
{}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  Compute CV
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

double Control_Variate::Compute_CV(double aux_payoff,double aux_explicit)
{
	return aux_payoff * discount_ - aux_explicit;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  end
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
