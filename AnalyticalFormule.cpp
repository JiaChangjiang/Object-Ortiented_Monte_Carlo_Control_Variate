//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  AnalyticalFormulae.cpp
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

#include "AnalyticalFormule.h"

#include "rv_library.h"

#include <cmath>
#include <iostream>

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	global constants
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

const double ROOT_THREE_INVERSE = 0.577350269189626;      // 1/sqr(3)
const double ROOT_PI_INVERSE = 0.564189583547756;        // 1/sqrt(PI)
const double ROOT_TWO_INVERSE = 0.707106781186547;       // 1/sqrt(2)

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	Analytic Solution For European Put Option
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

double AnalyticalFormulae::Euro_put_HullWhite_NewMethod
		(double S, double r, double X, double T, double mu, double m, double hull_white_sig, double y0)
{
	double c = mu + 0.5 * ( m - 1 ) * hull_white_sig * hull_white_sig;
	double b = y0 * ( exp ( c * T ) - 1 ) / c;
	double a = log(S) + r * T - 0.5*b;
	double d1 = ( log(X) - a ) / std::sqrt(b);
	
	return X * exp( - r * T ) * rv::normal_cdf(d1) - S * rv::normal_cdf( d1 - std::sqrt(b) );
}

double AnalyticalFormulae::Euro_put_HullWhite_MethodI
		(double S, double r, double X, double T, double mu, double m, double hull_white_sig, double y0)
{
	double c = mu + 0.25 * ( m - 2 ) * hull_white_sig * hull_white_sig;
	double b = y0 * ( exp( c * T ) - 1 ) / c;
	double a = log(S) + r * T - 0.5 * b;
	double d1 = ( log(X) - a ) / std::sqrt(b);
	
	return X * exp( - r * T ) * rv::normal_cdf(d1) - S * rv::normal_cdf( d1 - std::sqrt(b) );
}

double AnalyticalFormulae::Euro_put_HullWhite_MethodII
		(double S, double r, double X, double T, double mu, double m, double hull_white_sig, double y0)
{
	double c = mu - 0.25 * hull_white_sig * hull_white_sig;
	double b = y0 * ( exp( c * T ) - 1 ) / c;
	double a = log(S) + r * T - 0.5 * b;
	double d1 = ( log(X) - a ) / std::sqrt(b);
	
	return X * exp( - r * T ) * rv::normal_cdf(d1) - S * rv::normal_cdf( d1 - std::sqrt(b) );
}

double AnalyticalFormulae::Euro_put_Heston_NewMethod
		(double S, double r, double X, double T, double y0, double k, double theta)
{
	double c = ( y0 - theta ) / k * ( 1 - exp( - k * T ) ) + theta * T;
	double b = sqrt(c);
	double a = log(S) + r * T - 0.5 * c;
	double d1 = ( log(X) - a ) / b;
	
	return X * exp( - r * T ) * rv::normal_cdf(d1) - S * rv::normal_cdf(d1 - b);
}

double AnalyticalFormulae::Euro_put_Heston_MethodI
		(double S, double r, double X, double T, double y0, double k, double theta)
{
	double c = ( y0 - theta ) / k * ( 1 - exp( - k * T ) ) + theta * T;
	double b = sqrt(c);
	double a = log(S) + r * T - 0.5 * c;
	double d1 = ( log(X) - a ) / b;
	
	return X * exp( - r * T ) * rv::normal_cdf(d1) - S * rv::normal_cdf( d1 - b );
}

double AnalyticalFormulae::Euro_put_Heston_MethodII
		(double S, double r, double X, double T, double y0, double k, double theta)
{
	double c = ( y0 - theta ) / k * ( 1 - exp( - k * T ) ) + theta * T;
	double b = sqrt(c);
	double a = log(S) + r * T - 0.5 * c;
	double d1 = ( log(X) - a ) / b;
	
	return X * exp( - r * T ) * rv::normal_cdf(d1) - S * rv::normal_cdf( d1 - b );
}

double AnalyticalFormulae::Euro_put_Steinstein_NewMethod
		(double S, double r, double X, double T, double y0, double alpha, double beta)
{
	double b = beta * beta * T + 2 * beta * ( y0 - beta ) * ( exp( -alpha * T ) - 1 ) / -alpha
				+ ( y0 - beta ) * ( y0 - beta ) * ( exp( -2 * alpha * T ) - 1) / -alpha / 2;
	double a = log(S) + r * T - 0.5 * b;
	double c = sqrt(b);
	double d1 = ( log(X) - a ) / c;
	
	return X * exp( -r * T ) * rv::normal_cdf(d1) - S * rv::normal_cdf( d1 - c );
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	Analytic Solution For European Call Option
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

double AnalyticalFormulae::Euro_call_HullWhite_NewMethod
			(double S, double r, double X, double T, double mu, double m, double hull_white_sig, double y0)
{
	double p = Euro_put_HullWhite_NewMethod(S, r, X, T, mu, m, hull_white_sig, y0);
	
	return S + p - X * exp(-r * T);
}
	
double AnalyticalFormulae::Euro_call_HullWhite_MethodI
			(double S, double r, double X, double T, double mu, double m, double hull_white_sig, double y0)
{
	double p = Euro_put_HullWhite_MethodI(S, r, X, T, mu, m, hull_white_sig, y0);
	
	return S + p - X * exp(-r * T);
}
	
double AnalyticalFormulae::Euro_call_HullWhite_MethodII
			(double S, double r, double X, double T, double mu, double m, double hull_white_sig, double y0)
{
	double p = Euro_put_HullWhite_MethodII(S, r, X, T, mu, m, hull_white_sig, y0);
	
	return S + p - X * exp(-r * T);
}
	
double AnalyticalFormulae::Euro_call_Heston_NewMethod
			(double S, double r, double X, double T, double y0, double k, double theta)
{
	double p = Euro_put_Heston_NewMethod(S, r, X, T, y0, k, theta);
	
	return S + p - X * exp(-r * T);
}		

double AnalyticalFormulae::Euro_call_Heston_MethodI
			(double S, double r, double X, double T, double y0, double k, double theta)
{
	double p = Euro_put_Heston_MethodI(S, r, X, T, y0, k, theta);
	
	return S + p - X * exp(-r * T);
}
	
double AnalyticalFormulae::Euro_call_Heston_MethodII
			(double S, double r, double X, double T, double y0, double k, double theta)
{
	double p = Euro_put_Heston_MethodII(S, r, X, T, y0, k, theta);
	
	return S + p - X * exp(-r * T);
}

double AnalyticalFormulae::Euro_call_Steinstein_NewMethod
		(double S, double r, double X, double T, double y0, double alpha, double beta)
{
	double p = Euro_put_Steinstein_NewMethod(S, r, X, T, y0, alpha, beta);
	
	return S + p - X * exp(-r * T);
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	Analytic Solution For Geometric Asian Call Option
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

double AnalyticalFormulae::Asian_call_HullWhite_NewMethod
		(double S, double r, double X, double T, double y0, double mu, double hull_white_sig, double m)
{
	double a_m = mu + 0.5 * ( m - 1 ) * hull_white_sig * hull_white_sig;
	
	double a = 0;
	
	if (a_m == 0)
		a = log(S) + 0.5 * r * T - 0.25 * y0 * T;
	else
	{
		
		a = log(S) + 0.5 * r * T -y0 / ( 2 * T * a_m ) 
									* ( 1 / a_m * exp( a_m * T ) - T - 1 / a_m );
	}
	
	double sig_2 = 0;
	
	if (a_m == 0)
		sig_2 = y0 / T / 3;
	else
	{
		double ct = a_m * T;
        sig_2 = 2 * y0 / a_m * ( -0.5 + 1 / ct / ct * exp(ct) - 1 / ct - 1 / ct / ct );

	}
	
	double d_2 = ( a - log(X) ) / std::sqrt(sig_2);
	double d_1 = d_2 + std::sqrt(sig_2);
	
	return std::exp( 0.5 * sig_2 - r * T + a ) * rv::normal_cdf(d_1) 
								- X * std::exp( -r * T ) * rv::normal_cdf(d_2);
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  end
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
