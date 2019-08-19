//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//   rv_library.cpp
//   namespace RandomVariableStatisticalFunctions implementation file
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

#include "rv_library.h"

#include <iostream>
#include <cmath>
#include <cstdlib>
#include <limits>
#include <algorithm>
#include <ctime>
#include <stdexcept>

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//XX
//XX    n_probs( Z ):  Normal distribution probabilities accurate to 1.e-15
//XX
//XX       Alan Genz
//XX       Department of Mathematics
//XX       Washington State University
//XX       Pullman, Wa 99164-3113
//XX       Email : alangenz@wsu.edu
//XX
//XX     Based upon algorithm 5666 for the error function, from:
//XX     Hart, J.F. et al, 'Computer Approximations', Wiley 1968
//XX
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX


double RandomVariableStatisticalFunctions::n_probs(double z)
{
    static const double CUTOFF = 7.07106781186547;
      
    static const double p[7] =
	{
		220.206867912376,
		221.213596169931,
		112.079291497871,
		33.912866078383,
		6.37396220353165,
		0.700383064443688,
		3.52624965998911E-02
	};

    static const double q[8] =
	{
		440.413735824752,
		793.826512519948,
		637.333633378831,
		296.564248779674,
		86.7807322029461,
		16.064177579207,
		1.75566716318264,
		8.83883476483184E-02
	};
      
    double ZABS = std::fabs(z);

    double PP = 0;
    
    if (ZABS <= 37)
    {
        double EXPNTL = std::exp(-0.5*ZABS*ZABS);
        
        if (ZABS < CUTOFF)
        {
            double tempQ = ((q[7]*ZABS + q[6])*ZABS + q[5])*ZABS + q[4];
            tempQ = (((tempQ*ZABS + q[3])*ZABS + q[2])*ZABS + q[1])*ZABS + q[0];
            
            double tempP = ((p[6]*ZABS + p[5])*ZABS + p[4])*ZABS + p[3];
            tempP = ((tempP*ZABS + p[2])*ZABS + p[1])*ZABS + p[0];
            
            PP = EXPNTL*tempP/tempQ;
		}
        else
        {
            double tempQ = ZABS + 3.0/(ZABS + 4.0/(ZABS + 0.65));
            PP = (EXPNTL/(ZABS + 1.0/(ZABS + 2.0/tempQ)))/rv::ROOT_TWO_PI;
        }
    }
    
    return (z > 0) ? 1 - PP : PP;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//XX    normal pdf function
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

double RandomVariableStatisticalFunctions::normpdf(const double x, const double mu, const double sigma)
{
    double normed = (x - mu)/sigma;
    
    return std::exp(-0.5 * normed * normed) / (sigma*ROOT_TWO_PI);
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//XX    Hitting time density of a Brownian Motion (with time variable)
//XX    Bachelier and Levy Formula (Need to find reference) 
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

double RandomVariableStatisticalFunctions::hittingDist(const double a, const double b, const double t, const double mu, const double sigma)
{
    //static const double PI = 3.1415926535897932384626433832795;

    double tCube = t * t * t;
	double first = a / (sigma * std::sqrt(rv::TWO_PI * tCube));
	double third = a + t * (mu - b);
    double second = third * third / (2.0 * sigma * sigma * t);

	return first * std::exp(-second);
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//XX
//XX    Simulate from Hitting Distribution (IG distribution with negative drift)
//XX    Accept-Rejection method with Gamma Distributio n 
//XX	
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

double RandomVariableStatisticalFunctions::ranHittingDist
(
    const double a,
    const double b,
    const double drift,
    const double sigma
)
{
    double bad_value = -1;
    double mu = b - drift;
    double lambda = (a * a)/(sigma * sigma);
	double nu = a / std::abs(mu);

	double t = ranInverseGaussian(nu, lambda);

	if (mu >= 0) return t;  //just usual IG

	double probAccept = std::exp(-2.0 * lambda / nu);
	double u = my_ran2();

	return (u > probAccept) ? bad_value : t; 
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//XX    Inverse Guassian Probability Density Function
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

double RandomVariableStatisticalFunctions::InverseGaussianpdf(const double x, const double a, const double mu)
{
    double tCube = x * x * x;

    return a * std::exp(-0.5 * (a - mu*x)*(a - mu*x)/x) / std::sqrt(2.0*PI*tCube);
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//XX    Inverse Guassian Probability Density Function
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

double RandomVariableStatisticalFunctions::InverseGaussiancdf(const double x, const double lambda, const double nu)
{
    double rtlamb = std::sqrt(lambda / x);
    double first = rtlamb * (x / nu - 1.0);
    double second = rtlamb * (x / nu + 1.0);
      
    return normal_cdf(first) + std::exp(2.0 * lambda / nu) * normal_cdf(-second);
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//XX    Simulate from Inverse Guassian Distribution
//XX        
//XX    Root Transformation Method (MSH)
//XX	mu is a mean and a is a shape parameter.
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

double RandomVariableStatisticalFunctions::ranInverseGaussian(double nu, double lambda)
{
    double v = GetNormalVariate();
    double y = v * v;
    double nuy = nu * y;
    double lam2 = 2.0 * lambda;

    double x = nu * (1.0 + (nuy - std::sqrt(nuy * (4.0 * lambda + nuy))) /lam2);
    double u = my_ran2();
    
    if (u <= nu /(nu + x))
    {
       return x;
    }
    else
    {
       return (nu * nu)/x;
    }
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  ran0().  Modified from Numerical Recipes
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

double RandomVariableStatisticalFunctions::ran0()
{
	static bool NOT_FIRST_TIME = false;
	
	const static long IA = 16807, IM = 2147483647;
	const static long IQ = 127773;
	const long IR = 2836;
	const static long MASK = 123459876;
	const double AM = 1.0/double(IM);
	
	static long idum;
	
	if (NOT_FIRST_TIME == false)
	{
		srand(time(NULL));	
		idum = rand();
		NOT_FIRST_TIME = true;
	}
	
	idum ^= MASK;
	long k = idum/IQ;
	idum = IA*(idum - k*IQ) - IR*k;
	if (idum < 0) idum += IM;
	double ans = AM*idum;
	idum ^= MASK;
	return ans;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  double GetNormalVariate() Polar rejection.  Modified from Numerical Recipes
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

double RandomVariableStatisticalFunctions::GetNormalVariate()
{
	static bool Spare_normal_flag = false;
	static double Spare_normal;

	double rsq, v1, v2;

	if (Spare_normal_flag == false)
	{
		do
		{
			v1 = 2.0*my_ran2() - 1.0;			//Put in range (-1,1)
			v2 = 2.0*my_ran2() - 1.0;

			rsq = v1*v1 + v2*v2;

		} while (rsq >= 1.0 || rsq == 0.0);		//Reject if outside unit circle

		double fac = std::sqrt(-2.0*std::log(rsq)/rsq);

		Spare_normal = v1*fac; 					//Generates two normals
		Spare_normal_flag = true;  				//Store one,  return the other

		return v2*fac;

	}
	else
	{
		Spare_normal_flag = false;
		return Spare_normal;
	}
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  double my_ran2()   modified from Numerical recipes to use statics
//  Generates a U[0,1] with period ~2.3*10^18.
//	(See L'Ecuyer 1988 p747)
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

double RandomVariableStatisticalFunctions::my_ran2()   //modified from Numerical recipes
{
    static const long NTAB = 32;
    
    static const long IM1 = 2147483563;	static const long IA1 = 40014;
	static const long IQ1 = 53668;		static const long IR1 = 12211;
           
    static const long IM2 = 2147483399;	static const long IA2 = 40692;    
    static const long IQ2 = 52774;		static const long IR2 = 3791;
    
    static const long IMM1 = IM1 - 1;
    static const long NDIV = 1 + IMM1/NTAB;
    static const double EPS = 3.0e-16;
    static const double RNMX = 1.0 - EPS;
    static const double AM = 1.0/double(IM1);

	static long idum2;
	static long iy;
	static long iv[NTAB];
	static long idum;

	static bool NOT_FIRST_TIME = false;

	if (NOT_FIRST_TIME == false)		//run for first time
    {
		srand(time(NULL));	//initialise ran2
		idum = -rand();

		idum = (idum == 0)? 1 : -idum;
        idum2 = idum;

        for (long j = NTAB + 7; j >= 0; --j)
        {
            long k = idum/IQ1;
			idum = IA1*(idum - k*IQ1) - k*IR1;
            if (idum < 0) idum += IM1;
            
            if (j < NTAB) iv[j] = idum;
        }

        iy = iv[0];
        
        NOT_FIRST_TIME = true;
    }

    long k = idum/IQ1;
    idum = IA1*(idum - k*IQ1) - k*IR1;
    if (idum < 0) idum += IM1;
    
    k = idum2/IQ2;
    idum2 = IA2*(idum2 - k*IQ2) - k*IR2;
    if (idum2 < 0) idum2 += IM2;
    
    long j = iy/NDIV;
    iy = iv[j] - idum2;
    iv[j] = idum;

    if (iy < 1) iy += IMM1;

    double temp = AM*iy;
    
    return (temp > RNMX) ? RNMX : temp;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  max of two doubles
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

double RandomVariableStatisticalFunctions::maxd(double a, double b)
{
	return (a > b) ? a : b;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  min of two doubles
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

double RandomVariableStatisticalFunctions::mind(double a, double b)
{
	return (a < b) ? a : b;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//XX     cumulative univariate normal distribution.  just calls cody
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

double RandomVariableStatisticalFunctions::normal_cdf(double X)
{
	return 0.5 + 0.5*Cody_erf(X*ROOT_TWO_INVERSE, 0);
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//XX
//XX     double Cody_erf(double X, long jint)
//XX
//XX     This is a translation of a FORTRAN program by W. J. Cody,
//XX     Argonne National Laboratory, NETLIB/SPECFUN, March 19, 1990,
//XX     (incorporating minor amendments by C. Moler)
//XX     The main computation evaluates near-minimax approximations
//XX     from "Rational Chebyshev approximations for the error function"
//XX     by W. J. Cody, Math. Comp., 1969, PP. 631-638.
//XX
//XX         erf(x) = Cody_erf(x,0),  any x
//XX         erfc(x) = Cody_erf(x,1),  x < XBIG
//XX         erfcx(x) = exp(x^2)*erfc(x) = Cody_erf(x,2),  XNEG < x < XMAX
//XX
//XX     XMIN   = the smallest positive floating-point number.
//XX     XINF   = the largest positive finite floating-point number.
//XX     XNEG   = the largest negative argument acceptable to ERFCX;
//XX                the negative of the solution to the equation 2*exp(x*x) = XINF.
//XX     XSMALL = argument below which erf(x) may be represented by
//XX                 2*x/sqrt(pi)  and above which  x*x  will not underflow.
//XX                 A conservative value is the largest machine number X
//XX                 such that   1.0 + X = 1.0   to machine precision.
//XX     XBIG   = largest argument acceptable to ERFC;  solution to
//XX                the equation:  W(x) * (1-0.5/x**2) = XMIN,  where
//XX                W(x) = exp(-x*x)/[x*sqrt(pi)].
//XX     XHUGE  = argument above which  1.0 - 1/(2*x*x) = 1.0  to
//XX                machine precision.  A conservative value is 1 / [2*sqrt(XSMALL)]
//XX     XMAX   = largest acceptable argument to ERFCX;
//XX                the minimum of XINF and 1/[sqrt(pi)*XMIN]
//XX
//XX     The program returns  ERFC = 0      for  x >= XBIG;
//XX
//XX                          ERFCX = XINF  for  ARG < XNEG;
//XX
//XX                          ERFCX = 0     for  ARG >= XMAX.
//XX
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

double RandomVariableStatisticalFunctions::Cody_erf(double X, long jint)
{
	static const double XINF = 1.79769313486231E+308;         //Machine dependent constants;
	static const double XNEG = -26.6287357137515;
	static const double XSMALL = 1E-150;
	static const double XBIG = 25.0;
	static const double XHUGE = 5E+74;
	static const double XMAX = 5E+74;
	//static const double XMIN = 4.94065645841247E-324;


	static const double a[5] =          //Case |x| <= 0.46875
	{	3.16112374387057,
		113.86415415105,
		377.485237685302,
		3209.37758913847,
		0.185777706184603
	};

	static const double b[4] =          //Case |x| <= 0.46875
	{	23.6012909523441,
		244.024637934444,
		1282.61652607737,
		2844.23683343917
	};

	static const double c[9] =          //Case 0.46875 < |x| <= 4
	{	0.56418849698867,
		8.88314979438838,
		66.1191906371416,
		298.6351381974,
		881.952221241769,
		1712.04761263407,
		2051.07837782607,
		1230.339354798,
		2.15311535474404E-08
	};

	static const double D[8] =          //Case 0.46875 < |x| <= 4
	{	15.7449261107098,
		117.693950891312,
		537.18110186201,
		1621.38957456669,
		3290.79923573346,
		4362.61909014325,
		3439.36767414372,
		1230.33935480375
	};

	static const double P[6] =          //Case 4 < |x|
	{	0.305326634961232,
		0.360344899949804,
		0.125781726111229,
		1.60837851487423E-02,
		6.58749161529838E-04,
		1.63153871373021E-02
	};
    
	static const double Q[5] =          //Case 4 < |x|
	{	2.56852019228982,
		1.87295284992346,
		0.527905102951428,
		6.05183413124413E-02,
		2.33520497626869E-03
	};

	static const double xbreak_1 = 0.46875;
	static const long xbreak_2 = 4;

	double Cody = 0.0;    
	double z = 0.0;

	double Y = std::fabs(X);
        
	//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
	//XX     Evaluate erf for |x| <= 0.46875
	//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
    
	if (Y <= xbreak_1)
	{
		if (Y >= XSMALL) z = Y*Y;
        
		double xnum = a[4]*z;
		double xden = z;
        
		for (long i = 0; i <= 2; ++i)
		{
			xnum = (xnum + a[i])*z;
			xden = (xden + b[i])*z;
		}
        
		Cody = X*(xnum + a[3])/(xden + b[3]);
        
		if (jint != 0) Cody = 1 - Cody;

		if (jint == 2) Cody = std::exp(z)*Cody;

		return Cody;
	}
    
	//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
	//XX     Evaluate erf for 0.46875 < |x| <= 4
	//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
    
	if ((Y > xbreak_1) && (Y <= xbreak_2))
	{
		double xnum = c[8]*Y;
		double xden = Y;
        
		for(long i = 0; i <= 6; ++i)
		{
			xnum = (xnum + c[i])*Y;
			xden = (xden + D[i])*Y;
		}
        
		Cody = (xnum + c[7])/(xden + D[7]);
        
		if (jint != 2)
		{
			if (Y > 0) z = std::floor(Y * 16) / 16;
			else z = std::ceil(Y * 16) / 16;

			double del = (Y - z) * (Y + z);
			Cody = std::exp(-z * z) * std::exp(-del) * Cody;
		}
	}
    
	//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
	//XX     Evaluate erf for 4 < |x| < XBIG
	//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
          
	if ((Y > xbreak_2) && (Y < XBIG))
	{
		z = 1.0 / (Y * Y);
		double xnum = P[5] * z;
		double xden = z;
        
		for (long i = 0; i <= 3; ++i)
		{
		   xnum = (xnum + P[i]) * z;
		   xden = (xden + Q[i]) * z;
		}
        
		Cody = z * (xnum + P[4]) / (xden + Q[4]);
		Cody = (ROOT_PI_INVERSE - Cody) / Y;
         
		if (jint != 2)
		{
			if (Y > 0) z = std::floor(Y * 16) / 16;
			else z = std::ceil(Y * 16) / 16;

			double del = (Y - z) * (Y + z);
			Cody = std::exp(-z * z) * std::exp(-del) * Cody;
		}
	}
    
	//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
	//XX     Evaluate erf for XBIG <= |x|
	//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
          
	if ((Y >= XBIG) && ((jint != 2) || (Y >= XMAX))) Cody = 0;
    
	if ((Y >= XBIG) && (Y >= XHUGE)) Cody = ROOT_PI_INVERSE / Y;
 
	//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
	//XX     Fix up for negative argument, erf, etc.
	//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
    
	switch (jint)
	{
		case 0:
    
			if (X > xbreak_1) Cody = (0.5 - Cody) + 0.5;
			if (X < -xbreak_1) Cody = (-0.5 + Cody) - 0.5;
			break;
            
		case 1:
    
			if (X < -xbreak_1) Cody = 2.0 - Cody;
			break;
             
		case 2:
        
			if (X < -xbreak_1)
			{
				if (X < XNEG) Cody = XINF;
				else
				{
					if (Y > 0) z = std::floor(X * 16) / 16;
					else z = std::ceil(X * 16) / 16;

					double del = (X - z) * (X + z);
					Y = std::exp(z * z) * std::exp(del);
					Cody = (Y + Y) - Cody;
				}
			}
			break;
            
		default:

			//Argument must be 0, 1 or 2
			break;
	}

	return Cody;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//XX          cndev(u As Double) As Double.  Due to Moro 1994.
//XX          Computes Inverse normal distribution function
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

double RandomVariableStatisticalFunctions::cndev(double u)
{
	static const double a[4] =        //Constants in the rational polynomial approximation
	{	2.50662823884,
		-18.61500062529,
		41.39119773534,
		-25.44106049637
	};

	static const double b[4] =
	{	-8.4735109309,
		23.08336743743,
		-21.06224101826,
		3.13082909833
	};

	static const double c[9] =
	{	0.337475482272615,
		0.976169019091719,
		0.160797971491821,
		2.76438810333863E-02,
		3.8405729373609E-03,
		3.951896511919E-04,
		3.21767881768E-05,
		2.888167364E-07,
		3.960315187E-07
	};

	double X = u - 0.5;

	if (std::fabs(X) < 0.42)
	{
		double r = X * X;
		return X * (((a[3] * r + a[2]) * r + a[1]) * r + a[0]) / ((((b[3] * r + b[2]) * r + b[1]) * r + b[0]) * r + 1.0);
	}

	double r = (X > 0.0) ? 1.0 - u : u;

	r = std::log(-std::log(r));
    
	double r1 = r * (c[6] + r * (c[7] + r * c[8]));
	double r2 = r * (c[3] + r * (c[4] + r * (c[5] + r1)));
	double r3 = c[0] + r * (c[1] + r * (c[2] + r2));
    
	return (X < 0.0) ? -r3 : r3;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//
//      double ltqnorm(double p)
// Lower tail quantile for standard normal distribution function.
//
// This function returns an approximation of the inverse cumulative
// standard normal distribution function.  I.e., given P, it returns
// an approximation to the X satisfying P = Pr{Z <= X} where Z is a
// random variable from the standard normal distribution.
//
// The algorithm uses a minimax approximation by rational functions
// and the result has a relative error whose absolute value is less
// than 1.15e-9.
//
// Author:      Peter J. Acklam
// Time-stamp:  2002-06-09 18:45:44 +0200
// E-mail:      jacklam@math.uio.no
// WWW URL:     http://www.math.uio.no/~jacklam
//
// C implementation adapted from Peter's Perl version
//
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

double RandomVariableStatisticalFunctions::ltqnorm(double p, int err)
{
	static const double XINF = 1.79769313486231E+308; 	//Machine dependent constants

	static const double LOW = 0.02425;
	static const double HIGH = 0.97575;

	static const int ERR_DOM = 1;
	static const int ERR_RANGE = 2;

	err = 0;

    /* Coefficients in rational approximations. */

    static const double a[] =
    {
        -3.969683028665376e+01,
        2.209460984245205e+02,
        -2.759285104469687e+02,
        1.383577518672690e+02,
        -3.066479806614716e+01,
        2.506628277459239e+00
    };

    static const double b[] =
    {
        -5.447609879822406e+01,
        1.615858368580409e+02,
        -1.556989798598866e+02,
        6.680131188771972e+01,
        -1.328068155288572e+01
    };

    static const double c[] =
    {
        -7.784894002430293e-03,
        -3.223964580411365e-01,
        -2.400758277161838e+00,
        -2.549732539343734e+00,
        4.374664141464968e+00,
        2.938163982698783e+00
    };

    static const double d[] =
    {
        7.784695709041462e-03,
        3.224671290700398e-01,
        2.445134137142996e+00,
        3.754408661907416e+00
    };

    if (p < 0 || p > 1)
    {
		err = ERR_DOM;
        return 0.0;
    }
    else if (p == 0)
	{
		err = ERR_RANGE;
		return -XINF /* minus "infinity" */;
    }
    else if (p == 1)
    {
		err = ERR_RANGE;
		return XINF /* "infinity" */;
    }
    else if (p < LOW)
    {
        /* Rational approximation for lower region */
        double q = std::sqrt(-2.0*std::log(p));
        return (((((c[0]*q + c[1])*q + c[2])*q + c[3])*q +c [4])*q + c[5]) /
            ((((d[0]*q + d[1])*q + d[2])*q + d[3])*q + 1.0);
    }
    else if (p > HIGH)
    {
        /* Rational approximation for upper region */
        double q  = std::sqrt(-2.0*std::log(1.0 - p));
        return -(((((c[0]*q + c[1])*q + c[2])*q + c[3])*q + c[4])*q + c[5]) /
            ((((d[0]*q + d[1])*q + d[2])*q + d[3])*q + 1.0);
    }
    else
    {
        /* Rational approximation for central region */
        double q = p - 0.5;
        double r = q*q;
        return (((((a[0]*r + a[1])*r + a[2])*r + a[3])*r + a[4])*r + a[5])*q /
            (((((b[0]*r + b[1])*r + b[2])*r + b[3])*r + b[4])*r + 1.0);
    }
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//
//      double N(double z)   Fast but not very accurate.
//              cumulative univariate normal distribution.
//              This is a numerical approximation to the normal distribution.
//              See Abramowitz and Stegun: Handbook of Mathemathical functions
//              for description.  The arguments to the functions are assumed
//              normalized to a (0,1 ) distribution.
//
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

double RandomVariableStatisticalFunctions::N(double z)
{
    static double b1 =  0.31938153;
    static double b2 = -0.356563782;
    static double b3 =  1.781477937;
    static double b4 = -1.821255978;
    static double b5 =  1.330274429;
    static double p  =  0.2316419;
    static double c2 =  0.3989423;

    if (z >  6.0) { return 1.0; };  // this guards against overflow
	if (z < -6.0) { return 0.0; };  // This is a very narrow range

    double a = std::fabs(z);
    double t = 1.0/(1.0 + a*p);
    double b = c2*std::exp(-0.5*z*z);
    double n = ((((b5*t + b4)*t + b3)*t + b2)*t + b1)*t;

    n = 1.0 - b*n;

    return (z < 0.0) ? 1.0 - n : n;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//XX
//XX     void sobseq(const long n_dim, double *x)
//XX		Generates sobol' sequence numbers.  Modified from Numerical Recipes
//XX     	n_dim is number of dimensions,  results are returned in x()
//XX
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

void RandomVariableStatisticalFunctions::sobseq(std::vector<double> & x)
{
	const int MAXBIT = 30, MAXDIM = 39; //NR uses MAXDIM = 6

	static bool NOT_FIRST_TIME = false;
		
	static unsigned long step_number = 0;   //step_number incremented on every call of sobseq
	
	static long n_dim = x.size();

	static long r = (n_dim < MAXDIM) ? n_dim : MAXDIM;

	//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
	//XX	Uses Bratley and Fox numbers
	//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

	static const int degrees[MAXDIM] =   			//Degree of the primitive polynomials
		{1, 2, 3, 3, 4, 4, 5, 5, 5, 5,
		 5, 5, 6, 6, 6, 6, 6, 6, 7, 7,
		 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
		 7, 7, 7, 7, 7, 7, 8, 8, 8
		};
	static const unsigned long prim_poly[MAXDIM] = 	//Specifier of the primitive polynimial
		{ 0,  1,  1,  2,  1,  4,  2, 13,  7, 14,
		 11,  4,  1, 16, 13, 22, 19, 25,  1, 32,
		  4,  8,  7, 56, 14, 28, 19, 50, 21, 42,
		  31, 62, 37, 41, 55, 59, 14, 56, 21
		 };

		//B and F:  (3,7,11,13,19,25,37,59,47,            //NR = (BF - (1 + 2^degree))/2
		//			 61,55,41,67,97,91,109,103,115,131,
		//			 193,137,145,143,241,157,185,167,229,171,
		//			 213,191,253,203,211,239,247,285,369,299}

	static unsigned long coeff_values[MAXBIT*MAXDIM] =  //coefficients of primitive polynomials
		{	 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
			 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
			 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
			 1, 1, 1, 1, 1, 1, 1, 1, 1,

			 0, 1, 3, 1, 3, 1, 3, 3, 1,
			 3, 1, 3, 1, 3, 1, 1, 3, 1, 3,
			 1, 3, 1, 3, 3, 1, 3, 1, 3, 1,
			 3, 1, 1, 3, 1, 3, 1, 3, 1, 3,

			 0, 0, 7, 5, 1, 3, 3, 7, 5,
			 5, 7, 7, 1, 3, 3, 7, 5, 1, 1,
			 5, 3, 3, 1, 7, 5, 1, 3, 3, 7,
			 5, 1, 1, 5, 7, 7, 5, 1, 3, 3,

			 0, 0, 0, 0, 1, 7, 9, 13, 11,
			 1, 3, 7, 9, 5, 13, 13, 11, 3, 15,
			 5, 3, 15, 7, 9, 13, 9, 1, 11, 7,
			 5, 15, 1, 15, 11, 5, 3, 1, 7, 9,

			 0,   0,  0,  0,  0,  0,  9,  3, 27,
			 15, 29, 21, 23, 19, 11, 25,  7, 13, 17,
			 1,  25, 29,  3, 31, 11,  5, 23, 27, 19,
			 21,  5,  1, 17, 13,  7, 15,  9, 31,  9,

			 0,  0,  0,   0,  0,  0,  0,  0,  0,
			 0,  0,  0,  37, 33,  7,  5, 11, 39, 63,
			 27, 17, 15, 23, 29,  3, 21, 13, 31, 25,
			 9,  49, 33, 19, 29, 11, 19, 27, 15, 25,

			 0,  0,  0,   0,  0,  0,  0,  0,  0,
			 0,  0,  0,   0,  0,  0,  0,  0,  0, 13,
			 33,115, 41, 79, 17, 29,119, 75, 73,105,
			 7, 59,  65,  21, 3,113, 61, 89, 45,107,

			 0,  0,  0,   0,  0,  0,  0,  0,  0,
			 0,  0,  0,   0,  0,  0,  0,  0,  0, 0,
			 0,  0,  0,   0,  0,  0,  0,  0,  0, 0,
			 0,  0,  0,   0,  0,  0,  0,  7, 23, 39
		};

	static unsigned long x_big[MAXDIM];  //ix = x_big   Successive (unscaled) sobol sequence numbers
	static unsigned long *iu[MAXBIT];

	static double factor = 1.0/(1 << MAXBIT);		//scale factor of 2^-MAXBIT to create double in range [0,1]

	if (NOT_FIRST_TIME == false)
	{
		//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
		//XX     Just initialises everything.  Nothing returned.  Uses Bratley and Fox numbers
		//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

		for (long k = 0; k != MAXDIM; ++k) x_big[k] = 0;
						
		if (coeff_values[0] != 1) return;

		for (long j = 0, k = 0 ; j != MAXBIT; ++j, k += MAXDIM) iu[j] = &coeff_values[k];

		for (long k = 0; k != MAXDIM; ++k)
        {
			for (long j = 0; j != degrees[k]; ++j)           //Just normalise the given values
				iu[j][k] <<= (MAXBIT-1-j);

			for (long j = degrees[k]; j != MAXBIT; ++j)      //Work out the rest by recursion
            {
				unsigned long ipp = prim_poly[k];
				unsigned long i = iu[j - degrees[k]][k];
				i ^= (i >> degrees[k]);

				for (long l = degrees[k] - 1; l >= 1; l--)
                {
					if (ipp & 1) i ^= iu[j-l][k];      //If 1 in l'th place
					ipp >>= 1;
				}
				iu[j][k] = i;
			}
		}
		
		NOT_FIRST_TIME = true;
	}

	unsigned long this_step = step_number++;
	long count = 0;
	for (long j = 0; j != MAXBIT; ++j)		//Finds rightmost zero
    {
		if (!(this_step & 1)) break;
		this_step >>= 1;                //j is index of RH zero when exits for
		++count;
	}

	//if (count >= MAXBIT) nrerror("MAXBIT too small in sobseq");

	this_step = count*MAXDIM;

	for (long k = 0; k != r; ++k)
    {
		x_big[k] ^= coeff_values[this_step + k];
		x[k] = x_big[k]*factor;
	}
	
	for (long k = r; k != n_dim; ++k)
	    x[k] = my_ran2();
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//XX	Get_Brownian_bridge
//XX    Constructs a Brownian bridge between i and N, in binary chops,
//XX    Uses recursion to get lefthand and righthand sides.
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

void RandomVariableStatisticalFunctions::Get_Brownian_bridge
(
    std::vector<double> & Wiener_Path,
    const double dt,
    const long i, const long j, const long N
) 
{
    if (i == j || j == N) return;
	
	Wiener_Path[j] = Get_one_Wiener_bridge_step(Wiener_Path[i], Wiener_Path[N], dt*(j - i), dt*(N - j));
	
	if (j > i + 1) Get_Brownian_bridge(Wiener_Path, dt, i, long(std::floor(0.5 + 0.5*(i + j))), j);
	if (j < N - 1) Get_Brownian_bridge(Wiener_Path, dt, j, long(std::floor(0.5 + 0.5*(j + N))), N);
}

double RandomVariableStatisticalFunctions::Get_one_Wiener_bridge_step
(
	const double prior_z, 
	const double post_z, 
	const double prior_t, 
	const double post_t
)
{
	double interval = prior_t + post_t;
	
    double c_1 = post_t/interval;
	double c_2 = prior_t/interval;
	double c_3 = std::sqrt(prior_t*post_t/interval);

	double z = rv::cndev(rv::my_ran2());
    
	return c_1*prior_z + c_2*post_z + c_3*z;
	
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//XX	Get_Brownian_bridge
//XX    Constructs a Brownian bridge using given set of normal_rvs
//XX    Adapted from Jaeckel, P
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

void RandomVariableStatisticalFunctions::Get_Brownian_bridge
(
    std::vector<double> & Wiener_Path, 
    const double dt,
    const std::vector<double> & Normal_Variates
)
{
    long NumberOfSteps = Wiener_Path.size() - 1;
    
    if (NumberOfSteps <= 0 ||(int) Normal_Variates.size() != NumberOfSteps)
       throw std::runtime_error("Get_Brownian_bridge: there must be at least one step.");
    
    std::vector<long> leftIndex(NumberOfSteps), rightIndex(NumberOfSteps), 
                      bridgeIndex(NumberOfSteps), map(NumberOfSteps, 0);
    std::vector<double> leftWeight(NumberOfSteps), rightWeight(NumberOfSteps), 
                        stddev(NumberOfSteps);
    
    map[NumberOfSteps-1] = 1;
    bridgeIndex[0] = NumberOfSteps;
    stddev[0] = std::sqrt(NumberOfSteps * dt);
    leftWeight[0] = rightWeight[0] = 0.0;
    
    //precomputing stuff
    long i, j, k, l;
    for (j = 0, i = 1; i != NumberOfSteps; ++i)
    {
        while (map[j]) ++j;         
        k = j;                        //j is the first index not done
        while (!map[k]) ++k;          //k is the first index done
        l = j + ((k - 1 - j) >> 1);   //middle point index
        map[l] = i;                   //set index l done
        
        bridgeIndex[i] = l + 1;           
        leftIndex[i] = j;
        rightIndex[i] = k + 1;
        
        leftWeight[i] = (k - l) / (k + 1.0 - j);
        rightWeight[i] = (l + 1.0 - j) / (k + 1.0 - j);
        stddev[i] = std::sqrt(dt * (l + 1.0 - j) * (k - l) / (k + 1.0 - j));
        
        j = k + 1;
        if (j >= NumberOfSteps) j = 0;
    }
    
    //construct path
    Wiener_Path[0] = 0.0;
    Wiener_Path[NumberOfSteps] = stddev[0] * Normal_Variates[0];
    for (i = 1; i != NumberOfSteps; ++i)
    {
        j = leftIndex[i];
        k = rightIndex[i];
        l = bridgeIndex[i];
        
//      std::cout << l << ": " << j << " " << k << std::endl;
        
//        std::cout << l << ": " << leftWeight[i] << " " << rightWeight[i]
//                  << " " << stddev[i] << std::endl;
//                          
        if (j) Wiener_Path[l] = leftWeight[i] * Wiener_Path[j] +
              rightWeight[i] * Wiener_Path[k] + stddev[i] * Normal_Variates[i];
        else   Wiener_Path[l] = rightWeight[i] * Wiener_Path[k] + 
              stddev[i] * Normal_Variates[i];
    }
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//     NC_Chi_squ_cdf (from Numerical Recipes)
//     Returns the non-central chi-squared distribution function with v dof,  non_c of lam.  
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

double RandomVariableStatisticalFunctions::NC_Chi_squ_cdf(const double x, double const v, double const lam)
{
	const long MAX_ITERATIONS = 100;
	const double TOLERANCE = 0.00000001;
	
    if (x < 0 || v <= 0) throw std::runtime_error("NC_Chi_squ_dist: Arguments out of range");
    
    double lam2 = 0.5 * lam;
    double coeff = std::exp(-lam2);
    double sum = coeff * Chi_squ_cdf(x, v);   //chi_squ
    
    long i = 1;
    while (i < MAX_ITERATIONS)
    {
        coeff = coeff*lam2/i;
        double next_term = Chi_squ_cdf(x, v + 2.0 * i) * coeff;
    
        sum += next_term;
        
        if (std::fabs(next_term) < TOLERANCE) break;
    
        i = i + 1;
    }
    
    return sum;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//     Chi_squ_pdf
//     computes the Chi squared density function at x with a dof.  from Numerical Recipes
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

double RandomVariableStatisticalFunctions::Chi_squ_pdf(const double x, const double a)
{
     if (x <= 0 || a <= 0) throw std::runtime_error("Chi_squ_pdf: Arguments out of range");
     double fac = 0.693147180559945309 * (0.5 * a) + gammln(0.5 * a);
     
     return std::exp(-0.5 * (x - (a - 2.0) * std::log(x)) - fac);
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//     Chi_squ_cdf
//     computes the Chi squared distribution function at x with a dof.  from Numerical Recipes
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

double RandomVariableStatisticalFunctions::Chi_squ_cdf(const double x, const double a)
{                            
    return gammp(0.5 * a, 0.5 * x);
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//     Chi_squ_dist_inv
//     computes the inverse of Chi squared distribution function at u with a dof.  from Numerical Recipes
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

double RandomVariableStatisticalFunctions::Chi_squ_dist_inv(const double u, const double a)
{
    if (u < 0.0 || u >= 1.0) throw std::runtime_error("Chi_squ_dist_inv: Arguments out of range");
    return 2.0 * invgammp(u , 0.5 * a);
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//     double gammp(const double a, const double x)
//     Returns the imcomplete gamma function, P(a,x).  from Numerical Recipes
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

double RandomVariableStatisticalFunctions::gammp(const double a, const double x)
{        
    if (x < 0 || a <= 0) throw std::runtime_error("gammp: Arguments out of range");
              
    if (x < a + 1)    
        return gser(a, x);
    else
        return 1.0 - gcf(a, x);
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//     double invgammp(const double u, const double x)
//     Returns the inverse of imcomplete gamma function, P(a,x).  from Numerical Recipes
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

double RandomVariableStatisticalFunctions::invgammp(const double p, const double a)
{
    int j;
    double x, err, t, u, pp, lna1 = 0.0, afac = 0.0, a1 = a - 1.0;
    const double EPS = std::numeric_limits<double>::epsilon();
    double gln = gammln(a);  //strange
    
    if (a <= 0.0) throw std::runtime_error("invgammp: dof must be pos in invgammap");
    if (p >= 1.0) return std::max(100.0 ,a + 100.0 * std::sqrt(a));
    if (p <= 0.0) return 0.0;
    if (a > 1.0) { 
        lna1 = std::log(a1);
        afac = std::exp(a1 * (lna1 - 1.0) - gln);
        pp = (p < 0.5)? p : 1.0 - p;
        t = std::sqrt(-2.0 * std::log(pp));
        x = (2.30753 + t * 0.27061) / (1.0 + t * (0.99229 + t * 0.04481)) - t;
        if (p < 0.5) x = -x;
        x = std::max(1.0e-3, a * std::pow(1.0 - 1.0 / (9.0 * a) - x / (3.0 * std::sqrt(a)),3.0));
    } else { 
        t = 1.0 - a * (0.253 + a * 0.12); 
        if (p < t) x = std::pow(p / t, 1.0 / a);
        else x = 1.0 - std::log(1.0 -(p - t) / (1.0 - t));
    }
    
    for (j = 0; j < 12; j++) {
        if (x <= 0.0) return 0.0;
        err = gammp(a,x) - p;
        if (a > 1.0) t = afac * std::exp(-(x - a1) + a1 * (std::log(x) - lna1));
        else t = std::exp(-x + a1 * std::log(x) - gln);
        u = err / t;
        x -= (t = u /(1.0 - 0.5 * std::min(1.0, u * ((a - 1.0) / x - 1.0)))); 
        if (x <= 0.0) x = 0.5 * (x + t); 
        if (fabs(t) < EPS * x ) break;
    }
    return x;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//     double RandomVariableStatisticalFunctions::gser(const double a, const double x)
//     Slightly modified from Numerical Recipes
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

double RandomVariableStatisticalFunctions::gser(const double a, const double x)
{
	const long ITMAX = 100;
	const double EPS = std::numeric_limits<double>::epsilon();

	double gln = gammln(a);
	if (x <= 0.0) throw std::runtime_error("gser: x out of range");

	double ap = a;
	double del = 1.0/a;
	double sum = 1.0/a;
	for (long n = 0; n < ITMAX; ++n) 
	{
		++ap;
		del *= x/ap;
		sum += del;
		if (std::fabs(del) < std::fabs(sum)*EPS) return sum*std::exp(-x + a*std::log(x) - gln);
	}

	long aa = 1; long bb = 0;
	if (aa == bb) return 0.0;
	else throw std::runtime_error("gser: a too large, ITMAX too small in routine gser");
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//     double RandomVariableStatisticalFunctions::gser(const double a, const double x)
//     Slightly modified from Numerical Recipes
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

double RandomVariableStatisticalFunctions::gcf(const double a, const double x)
{
	const long ITMAX = 100;
	const double EPS = std::numeric_limits<double>::epsilon();
	const double FPMIN = std::numeric_limits<double>::min()/EPS;

	double gln = gammln(a);
	double b = x + 1.0 - a;
	double c = 1.0/FPMIN;
	double d = 1.0/b;
	double h = d;
	
	long i = 1;
	while (i <= ITMAX) 
	{
		double an = -i*(i - a);
		b += 2.0;
		d = an*d + b;
		if (std::fabs(d) < FPMIN) d = FPMIN;
		c = b + an/c;
		if (std::fabs(c) < FPMIN) c = FPMIN;
		d = 1.0/d;
		double del = d*c;
		h *= del;
		if (std::fabs(del - 1.0) <= EPS) break;
		
		++i;
	}
	
	if (i > ITMAX) throw std::runtime_error("gcf:  a too large, ITMAX too small");
	
	return std::exp(-x + a*std::log(x) - gln)*h;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//     double RandomVariableStatisticalFunctions::gammln(const double xx)
//     Slightly modified from Numerical Recipes
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

double RandomVariableStatisticalFunctions::gammln(const double xx)
{
	static const double cof[6] = 
		{
			76.18009172947146, 
			-86.50532032941677,
			24.01409824083091,
			-1.231739572450155,
			0.1208650973866179e-2,
			-0.5395239384953e-5
		};

	double y = xx;
	double x = xx;
	double tmp = x + 5.5;
	tmp -= (x + 0.5)*std::log(tmp);
	double ser = 1.000000000190015;
	
	for (long j = 0; j < 6; ++j) ser += cof[j]/++y;
	
	return -tmp + std::log(2.5066282746310005*ser/x);
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//     bessel_pdf(x, xnu)
//     Computes probability mass function of Bessel distribution
//     x >= 0, xnu is v >= 0
//     
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

long RandomVariableStatisticalFunctions::rand_bessel(const double x, const double xnu)
{
    static const long NMAX = 1000;
        
    double prod = std::pow(0.5 * x, xnu) / (besseli(x, xnu) * std::exp(gammln(xnu + 1.0)));
    double u = my_ran2();
    double sum = 0.0;
    
    for (long i = 0; i <= NMAX; ++i)
    {
         sum += prod;
         if (u < sum) return i;
         else prod *= x * x / (4 * (i + 1) * (i + 1 + xnu)); 
    }
    
    return NMAX;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//     bessel1(x, xnu)
//     Numerical recipes code.  
//     Computes modified bessel functions of the first kind, I(x, xnu). Uses beschb
//     x >= 0, xnu is v >= 0
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

double RandomVariableStatisticalFunctions::besseli(const double x, const double xnu)
{
	double i, k, di, dk;
	
	if (xnu >= 0.0) bessik(x, xnu, i, k, di, dk);
	else 
	{
         bessik(x, -xnu, i, k, di, dk);
         i = i - TWO_PI_INVERSE * std::sin(xnu * PI) * k;
    }
     
    return i;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//     bessik
//     Numerical recipes code.  Computes modified bessel functions and derivatives. Uses beschb
//     x >= 0, xnu is v >= 0
//     ri = I(x,xnu), rk = K(x,xnu), rip = I'(x,xnu), rkp = K'(x,xnu)
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

void RandomVariableStatisticalFunctions::bessik(const double x, const double nu, double & io, double & ko, double & ipo, double & kpo)
{
	static const long MAXIT = 10000;
	static const double EPS = std::numeric_limits<double>::epsilon();
	static const double FPMIN = std::numeric_limits<double>::min()/EPS;
	static const double XMIN = 2.0;
    static const double PI = 3.141592653589793;

    double a,a1,b,c,d,del,del1,delh,dels,e,f,fact,fact2,ff,gam1,gam2,
     gammi,gampl,h,p,pimu,q,q1,q2,qnew,ril,ril1,rimu,rip1,ripl,
     ritemp,rk1,rkmu,rkmup,rktemp,s,sum,sum1,x2,xi,xi2,xmu,xmu2;
    long i,l,nl;
    if (x <= 0.0 || nu < 0.0) throw std::runtime_error("bad arguments in besselik");
    nl=long(nu+0.5);
    xmu=nu-nl;
    
    xmu2=xmu*xmu;
    xi=1.0/x;
    xi2=2.0*xi;
    h=nu*xi; 
    if (h < FPMIN) h=FPMIN; 
    b=xi2*nu;
    d=0.0;
    c=h;
    for (i=0;i<MAXIT;i++) 
    {
         b += xi2;
         d=1.0/(b+d); 
         c=b+1.0/c;
         del=c*d;
         h=del*h;
         if (std::fabs(del-1.0) <= EPS) break;
    }
    
    if (i >= MAXIT)
    throw std::runtime_error("x too large in besselik; try asymptotic expansion");
    ril=FPMIN;
    ripl=h*ril; 
    ril1=ril; 
    rip1=ripl;
    fact=nu*xi;
    for (l=nl-1;l >= 0;l--)
    {
         ritemp=fact*ril+ripl;
         fact -= xi;
         ripl=fact*ritemp+ril;
         ril=ritemp;
    }
    
    f=ripl/ril; 
    if (x < XMIN) 
    { 
        x2=0.5*x;
        pimu=PI*xmu;
        fact = (std::fabs(pimu) < EPS ? 1.0 : pimu/sin(pimu));
        d = -log(x2);
        e=xmu*d;
        fact2 = (std::fabs(e) < EPS ? 1.0 : sinh(e)/e);
        beschb(xmu, gam1, gam2, gampl, gammi);
        ff=fact*(gam1*std::cosh(e)+gam2*fact2*d); 
        sum=ff;
        e=exp(e);
        p=0.5*e/gampl; 
        q=0.5/(e*gammi);
        c=1.0;
        d=x2*x2;
        sum1=p;
        for (i=1;i<=MAXIT;i++) 
        {
             ff=(i*ff+p+q)/(i*i-xmu2);
             c *= (d/i);
             p /= (i-xmu);
             q /= (i+xmu);
             del=c*ff;
             sum += del;
             del1=c*(p-i*ff);
             sum1 += del1;
             if (std::fabs(del) < std::fabs(sum)*EPS) break;
        }
        if (i > MAXIT) throw std::runtime_error("bessk series failed to converge");
        rkmu=sum;
        rk1=sum1*xi2;
    } 
    else 
    { 
        b=2.0*(1.0+x);
        d=1.0/b;
        h=delh=d;
        q1=0.0; 
        q2=1.0;
        a1=0.25-xmu2;
        q=c=a1; 
        a = -a1;
        s=1.0+q*delh;
        for (i=1;i<MAXIT;i++) 
        {
             a -= 2*i;
             c = -a*c/(i+1.0);
             qnew=(q1-b*q2)/a;
             q1=q2;
             q2=qnew;
             q += c*qnew;
             b += 2.0;
             d=1.0/(b+a*d);
             delh=(b*d-1.0)*delh;
             h += delh;
             dels=q*delh;
             s += dels;
             if (std::fabs(dels/s) <= EPS) break;
        }
        if (i >= MAXIT) throw std::runtime_error("besselik: failure to converge in cf2");
        h=a1*h;
        rkmu=sqrt(PI/(2.0*x))*exp(-x)/s; 
        rk1=rkmu*(xmu+x+0.5-h)*xi;
    }
    rkmup=xmu*xi*rkmu-rk1;
    rimu=xi/(f*rkmu-rkmup); 
    io=(rimu*ril1)/ril; 
    ipo=(rimu*rip1)/ril;
    for (i=1;i <= nl;i++) 
    { 
         rktemp=(xmu+i)*xi2*rk1+rkmu;
         rkmu=rk1;
         rk1=rktemp;
    }
    ko=rkmu;
    kpo=nu*xi*rkmu-rk1;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//     beschb. modified from numerical recipes.
//     evaluate gamma function by chebyshev expansion.  Uses chebev
//     gam1 = gamma_1,  gam2 = gamma_2,  gamp1 = 1/gamma(1+x),  gammi = 1/gamma(1-x).
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

void RandomVariableStatisticalFunctions::beschb(const double x, double &gam1, double &gam2, double &gampl, double &gammi)
{
	const long NUSE1 = 7;
	const long NUSE2 = 8;
	
	static const double c1_d[NUSE1] = 
	{
		-1.142022680371168e0, 
		6.5165112670737e-3,
		3.087090173086e-4, 
		-3.4706269649e-6,
		6.9437664e-9, 
		3.67795e-11,
		-1.356e-13
	};
	
	static const double c2_d[NUSE2] = 
	{
		1.843740587300905e0, 
		-7.68528408447867e-2,
		1.2719271366546e-3, 
		-4.9717367042e-6,
		-3.31261198e-8, 
		2.423096e-10,
		-1.702e-13, 
		-1.49e-15
	};

	double xx = 8.0*x*x - 1.0;
	gam1 = chebev(-1.0, 1.0, c1_d, NUSE1, xx);
	gam2 = chebev(-1.0, 1.0, c2_d, NUSE2, xx);
	gampl = gam2 - x*gam1;
	gammi = gam2 + x*gam1;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//     chebev.  chebyshev evaluation ex Numerical Recipes
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

double RandomVariableStatisticalFunctions::chebev(const double  a, const double b, const double * const c, const long m, const double x)
{
	double d = 0.0;
	double dd = 0.0;
	
	if ((x - a)*(x - b) > 0.0) throw std::runtime_error("chebev: x not in range");
	
	double y = (2.0*x - a - b)/(b - a);
	double y2 = 2.0*y;
	for (long j = m-1; j > 0; --j) 
	{
		double sv = d;
		d = y2*d - dd + c[j];
		dd = sv;
	}
	return y*d - dd + 0.5*c[0];
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//XX    long Ex(const double lambda)
//XX	Simulates from an Exponential distribution (by inverse transform)
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

double RandomVariableStatisticalFunctions::Ex(const double lambda)
   {return -std::log(my_ran2()) * lambda;}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//XX    long Poisson_jumps(const double lambda)
//XX	Simulates a Poisson distribution (by inverse transform)
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

long RandomVariableStatisticalFunctions::Poisson_jumps(const double lambda)
{
    const long N_max = 1000;     //Arbitrary upper bound on the number of jumps
	double w = 1.0;
    double sum = 1.0;
    
    double u = rv::my_ran2()*std::exp(lambda);
    if (u < sum) return 0;
    
    for (long i = 1; i <= N_max; ++i)
    {    
        w *= lambda/i;        
        sum += w;
        
        if (u < sum) return i;
     }
     
     return N_max;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//XX    long Poisson_jumps_inv(const double lambda, const double u)
//XX	Simulates a Poisson disotribution (by inverse transform)
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

long RandomVariableStatisticalFunctions::Poisson_jumps_inv(const double lambda, const double u)
{
    const long N_max = 1000;     //Arbitrary upper bound on the number of jumps
	double w = 1.0;
    double sum = 1.0;
    
    double v = u * std::exp(lambda);
    if (v < sum) return 0;
    
    for (long i = 1; i <= N_max; ++i)
    {    
        w *= lambda/i;        
        sum += w;
        
        if (v < sum) return i;
     }
     
     return N_max;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//XX    long Poisson_jumps2(const double lambda)
//XX	Simulates a Poisson variate (rejection method), adapted from Numerical Recipe 3rd ed.
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

long RandomVariableStatisticalFunctions::Poisson_jumps2(const double lambda)
{
     long k;
     
     if (lambda < 5.0)
     {
        double lamexp = std::exp(-lambda);
        k = -1;
        double t = 1.0;
        
        do
        {
            ++k;
            t *= my_ran2();       
        }   while (t > lamexp);        
     }
     else
     {
        double sqlam = std::sqrt(lambda);
        double loglam = std::log(lambda);
        
        for (;;)
        {
            double u = 0.64 * my_ran2();
            double v = -0.68 + 1.28 * my_ran2();
            double v2 = v * v;
            
            if (lambda > 13.5)
            {               
               if (v >= 0.0) {if (v2 > 6.5 * u * (0.64 - u) * (u + 0.2)) continue;}
               else {if (v2 > 9.6 * u * (0.66 - u) * (u + 0.07)) continue;}
            }
            
            k = long(sqlam * (v / u) + lambda + 0.5);
            if (k < 0) continue;
            
            double u2 = u * u;
            
            if (lambda > 13.5)
            {
               if (v >= 0.0) {if (v2 < 15.2 * u2 * (0.61 - u) * (0.8 - u)) break;}
               else {if (v2 < 6.76 * u2 * (0.62 - u) * (1.4 - u)) break;}
            }
            
            double lfac = gammln(k + 1.0);  //need speed-up
            double p = sqlam * std::exp(-lambda + k * loglam - lfac);
            
            if (u2 < p) break;
        }   
     }
     
     return k;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//XX    Brownian bridge hitting time distribution to a linear barrier
//XX	Pr[tau < T | u_t = a + bt;  w_0, w_T] 
//XX	dw = mu.dt + sigma.dz
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

double RandomVariableStatisticalFunctions::Brownian_bridge_hitting_time_dist
(
	const double w0, const double wT, const double T, const double a, const double b,
	const double sigma
)
{
	double s2_T = sigma*sigma*T;
	return std::exp(-2.0*(a - w0)*(a - wT + b*T)/s2_T);
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//XX    Brownian bridge distribution of the maximum
//XX	Pr[M < B | w_0, w_T],  dw = mu.dt + sigma.dz
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

double RandomVariableStatisticalFunctions::Brownian_bridge_maximum_dist
(
	const double w0, const double wT, const double T, const double B, const double sigma
)
{
	if ((w0 >= B) || (wT >= B)) return 0.0;
	
	double s2_T = sigma*sigma*T;
	return 1 - std::exp(-2.0*(B - w0)*(B - wT)/s2_T);
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//XX    Brownian bridge distribution of the maximum
//XX	Pr[M < B | w_0, w_T],  dw = mu.dt + sigma.dz
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

double RandomVariableStatisticalFunctions::Brownian_bridge_minimum_dist
(
	const double w0, const double wT, const double T, const double B, const double sigma
)
{
	if ((w0 <= B) || (wT <= B)) return 1.0;
	
	double s2_T = sigma*sigma*T;
	return std::exp(-2.0*(B - w0)*(B - wT)/s2_T);
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//XX    Simulation from the Brownian bridge distribution of the maximum
//XX	f( M | w_0, w_T),  dw = mu.dt + sigma.dz
//XX	u is a uniform variate
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

double RandomVariableStatisticalFunctions::Brownian_bridge_maximum_draw
(
	const double w0, const double wT, const double T, const double sigma, const double u
)
{
	double s_rt_T = sigma*std::sqrt(T);
	double z0 = w0/s_rt_T;
	double zT = wT/s_rt_T;
	
	return s_rt_T*Wiener_bridge_maximum_draw(z0, zT, T, u);
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//XX    Simulation from the Wiener bridge distribution of the maximum
//XX	f( M | w_0, w_T),  dw = mu.dt + sigma.dz
//XX	u is a uniform variate
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

double RandomVariableStatisticalFunctions::Wiener_bridge_maximum_draw
(
	const double z0, const double zT, const double T, const double u
)
{
	return 0.5*(z0 + zT + std::sqrt((z0 - zT)*(z0 - zT) - 2.0*T*std::log(1 - u)));
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//XX    Simulation from the Brownian bridge distribution of the minimum
//XX	f( m | w_0, w_T),  dw = mu.dt + sigma.dz
//XX	u is a uniform variate
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

double RandomVariableStatisticalFunctions::Brownian_bridge_minimum_draw
(
	const double w0, const double wT, const double T, const double sigma, const double u
)
{
	double s_rt_T = sigma*std::sqrt(T);
	double z0 = w0/s_rt_T;
	double zT = wT/s_rt_T;
	
	return s_rt_T*Wiener_bridge_minimum_draw(z0, zT, T, u);
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//XX    Simulation from the Wiener bridge distribution of the minimum
//XX	f( m | w_0, w_T),  dw = mu.dt + sigma.dz
//XX	u is a uniform variate
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

double RandomVariableStatisticalFunctions::Wiener_bridge_minimum_draw
(
	const double z0, const double zT, const double T, const double u
)
{
	return 0.5*(z0 + zT - std::sqrt((z0 - zT)*(z0 - zT) - 2.0*T*std::log(1 - u)));
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//XX     rand_gamma.  This is gmrand ex Numerical Recipes
//XX
//XX     Gamma random number generator.  ia is the shape parameter in gamma density
//XX     Generates gamma(ia,1),  where scale parameter has been set to 1
//XX     (Gamma(ia,ib) is ib*gamma(ia))
//XX
//XX     If ia < 1, use gmdvlitt. If ia > 1, use gmdvgrea.
//XX     If ia = 1, use exponential rv generator.
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

double RandomVariableStatisticalFunctions::rand_gamma(double IA)
{
    if (IA < 1) return rv::rand_gamma_small(IA);
    if (IA > 1) return rv::rand_gamma_large(IA);

    return -std::log(rv::my_ran2());
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//XX     rand_gamma_small.  gmdvlitt ex Numerical Recipes
//XX
//XX     The random number generator for gamma distribution with ia <= 1.
//XX     Based on Best's algorithm (1983).  cf Devroye's "non-uniform random variate generation".
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

double RandomVariableStatisticalFunctions::rand_gamma_small(double lia)
{
    if (lia > 1)
    {
		throw std::runtime_error("rand_gamma_small: gamma parameter must be < 1");
	}
    
    bool accept = false;
    
    double T = 0.07 + 0.75*std::sqrt(1 - lia);
    double b = 1.0 + lia*std::exp(-T)/T;
    double c = 1.0/lia;
    
    double X;

    while (accept == false)
    {
        double U = rv::my_ran2();
        double W = rv::my_ran2();
        double V = b*U;
        
        if (V <= 1.0)
        {
            X = T * std::pow(V, c);  //mistake?
            if ((W <= (2.0 - X)/(2.0 + X)) || (W <= std::exp(-X)))
            	accept = true;
        } 
		else 
		{
            X = -std::log(c*T*(b - V));
			double y = X/T;
            if ((W*(lia + y - lia*y) <= 1.0) || (W <= std::pow(y, lia - 1.0)))
                  accept = true;
        }
    }
    
    return X;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//XX     rand_gamma_large.  gmdvgrea ex Numerical Recipes
//XX
//XX     The random number generator for gamma distribution with ia > 1.
//XX     Based on Best's algorithm (1978).  cf Devroye's "non-uniform random variate generation".
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

double RandomVariableStatisticalFunctions::rand_gamma_large(double gia)
{
    if (gia < 1.0)
    {
		throw std::runtime_error("rand_gamma_large: gamma parameter must be > 1");
	}
    
    bool accept = false;
    
    double b = gia - 1.0;
    double c = 3.0*gia - 0.75;
    
    double X;

    while (accept == false)
    {       
        double U = rv::my_ran2();
        double V = rv::my_ran2();
        double w = U*(1.0 - U);
        double y = (U - 0.5)*std::sqrt(c/w);
        X = b + y;
        
        if (X >= 0)
        {
            double z = 64.0*w*w*w*V*V;
            if ((z <= 1.0 - (2.0*y*y)/X) || (std::log(z) <= 2.0*(b*std::log(X/b) - y)))
                accept = true;
        }
	}
    
    return X;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//XX     rand_gamma(const double, const double).  Marsaglia and Tsang
//XX     Rejection method. Numerical recipe
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

double RandomVariableStatisticalFunctions::rand_gamma(double alpha, double beta)
{
    double oalpha = alpha;
    if (alpha <= 0.0) throw std::runtime_error("rand_gamma: bad alpha in Gamma variate");
    if (alpha < 1.0) alpha += 1.0;
    double a1 = alpha - 1.0 / 3.0;
    double a2 = 1.0 / std::sqrt(9.0 * a1);
    
    double u, v, x;
    do 
    {
       do 
       {
           x = cndev(my_ran2());
           v = 1.0 + a2 * x;
       } while (v <= 0.0);
       
       v = v * v * v;
       u = my_ran2();
    } while (u > 1.0 - 0.0331 * x * x * x * x && 
             std::log(u) > 0.5 * x * x + a1 * (1.0 - v + std::log(v)));   //rarely evaluated
             
    if (alpha == oalpha) return a1 * v / beta;
    else {                                                                //case where alpha < 1. per Ripley.
        do u = my_ran2(); while (u == 0);
        return std::pow(u, 1.0 / oalpha) * a1 * v / beta;
    }
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//XX          student0(v)
//XX          generates a student's t variate with v dof by polar rejection
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

double RandomVariableStatisticalFunctions::rand_student(double  V)
{  
    double w = 1;
    double u1 = 0.0, u2 = 0.0;       // to avoid a compiler warning
    while (w >= 1.0)                 // Finds a t(v) variate by Polar Rejection
    {
	    u1 = 2.0*rv::my_ran2() - 1.0;
        u2 = 2.0*rv::my_ran2() - 1.0;
        w = u1*u1 + u2*u2;
    }
	
	//Have not rejected,  so construct the student variate
        
    return u1*std::sqrt(V*(std::pow(w, -4.0/V) - 1.0)/w);
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//XX     rand_ig
//XX     Generates inverse gaussian random number by Michael, Schucany and Haas's algorithm.
//XX     See p 148 of Devroye's non-uniform random variate generation.
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

double RandomVariableStatisticalFunctions::rand_ig(double delta, double gamma)
{
    double mu = delta/gamma;
    double lambda = delta*delta;

    double N = rv::cndev(rv::my_ran2());
    double y = N*N*mu;
    
    double c_one = 0.5*mu/lambda;
    double c_two = y*(4.0*lambda + y);
    
    double X_one = mu + c_one*(y - std::sqrt(c_two));
    
    double U = rv::my_ran2();
    
    if (U <= mu/(mu + X_one)) return X_one;

    return mu*mu/X_one;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//XX    student 3 dof functions
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

double RandomVariableStatisticalFunctions::student_3_density(double x)
{
	double s = 3.0 + x*x;
	return 	6.0*rv::ROOT_THREE/(s*s*rv::PI);
}

double RandomVariableStatisticalFunctions::student_3_dist(double x)
{
	double s = 3.0 + x*x;
	double t = std::atan(x/rv::ROOT_THREE);
	return 0.5 + (t + rv::ROOT_THREE*x/(s*s))/rv::PI;
}

double RandomVariableStatisticalFunctions::student_3_dist_inv(double u)
{
	const double cross_over1 = 0.16;
	const double cross_over2 = 0.84;

	if ((u <= 0.0) || (u >= 1.0)) throw std::runtime_error("student_3_dist_inv: argument out of range");

	double x;	//the solution

	const double d[] =
	{
		-2.0/5.0,  -9.0/175.0,  -92.0/7875.0,
		-1894.0/606375.0, -19758.0/21896875.0, -2418092.0/8868234375.0
	};

	const double a[] =
	{
		2.0, 11.0, 292.0, 3548.0, 273766.0, 15360178.0,
		214706776.0, 59574521252.0, 15270220299064.0
	};

	const double b[] =
	{
		9.0, 135.0, 8505.0, 229635.0, 37889775.0, 4433103675.0,
		126947968875.0, 71217810538875.0, 36534736806442875.0
	};

	if ((u <= cross_over1) || (u >= cross_over2))
	{
		double z = (u >= cross_over2) ? u : 1.0 - u;

		double v = 3.0*rv::PI_BY_TWO*(1.0 - z);
		double w = std::pow(v, 2.0/3.0);
		double y = w;
		x = 1.0;

		for (long i = 0; i <= 5; ++i)
		{
			x += d[i]*y;
			y *= w;
		}
		x *= rv::ROOT_THREE*w/v;

		x = (u >= cross_over2) ? x : -x;
	}
	else
	{
		double v = rv::ROOT_THREE_PI_BY_TWO*(u - 0.5);
		double w = v*v;
		double y = w;
		x = 1.0;

		for (long i = 0; i <= 8; ++i)
		{
			x += a[i]*y/b[i];
			y *= w;
		}
		x *= v;
	}

	return x;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//XX    student 5 dof functions
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

double RandomVariableStatisticalFunctions::student_5_density(double x)
{
	double s = 5.0 + x*x;
	return 	200.0*rv::ROOT_FIVE/(3.0*s*s*s*rv::PI);
}

double RandomVariableStatisticalFunctions::student_5_dist(double x)
{
	double s = 5.0 + x*x;
	double t = std::atan(x/rv::ROOT_FIVE);
	return 0.5 + (t + rv::ROOT_FIVE*x*(3.0*x*x + 25.0)/(3.0*s*s))/rv::PI;
}

double RandomVariableStatisticalFunctions::student_5_dist_inv(double u)
{
	const double cross_over1 = 0.16;
	const double cross_over2 = 1.0 - cross_over1;

	if ((u <= 0.0) || (u >= 1.0)) throw std::runtime_error("student_5_dist_inv: argument out of range");

	double x;	//the solution

	const double d[] =
	{
		-3.0/7.0,  -10.0/147.0,  -75.0/3773.0,
		-7180.0/1030029.0, -13757.0/5150145.0, -65447468.0/60673858245.0,
		565996174.0/1255274711691.0
	};

	const double a[] =
	{
		1.0, 9.0, 134.0, 926.0, 40976.0, 1325803.0,
		354074884.0, 5186310638.0, 773723059913.0
	};

	const double b[] =
	{
		5.0, 125.0, 4375.0, 65625.0, 6015625.0, 391015625.0,
		205283203125.0, 5816357421875.0, 1657661865234375.0
	};

	if ((u <= cross_over1) || (u >= cross_over2))
	{
		double z = (u >= cross_over2) ? u : 1.0 - u;

		double w = 15.0*rv::PI*(1.0 - z)/8.0;
		double v1 = std::pow(w, 1.0/5.0);
		double v2 = v1*v1;
		double y = v2;
		x = 1.0;

		for (long i = 0; i <= 6; ++i)
		{
			x += d[i]*y;
			y *= v2;
		}
		x *= rv::ROOT_FIVE/v1;

		x = (u >= cross_over2) ? x : -x;
	}
	else
	{
		double v = 3.0*rv::ROOT_FIVE*rv::PI*(u - 0.5)/8.0;
		double w = v*v;
		double y = w;
		x = 1.0;

		for (long i = 0; i <= 8; ++i)
		{
			x += a[i]*y/b[i];
			y *= w;
		}
		x *= v;
	}

	return x;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//XX   cumulative distribution function for a bivariate normal distribution
//XX   John Hull: Options, Futures and Other Derivatives  
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

double RandomVariableStatisticalFunctions::ND2(double a, double b, double rho)
{
    double rho1;
    double rho2;
    double denominator;
    double result;

    if (rho > 0.9999)
        result = rv::N(rv::min(a, b));
    else if (rho < -0.9999)
        result = rv::max(0, N(a) - N(-b));
    else
    {
        if (a * b * rho <= 0) 
        {
            if (a <= 0 && b <= 0 && rho <= 0)
                result = rv::Ntwo(a, b, rho);
            else if (a <= 0 && b * rho >= 0)
                result = rv::N(a) - rv::Ntwo(a, -b, -rho);
            else if (b <= 0 && rho >= 0)
                result = rv::N(b) - rv::Ntwo(-a, b, -rho);
            else
                result = rv::N(a) + rv::N(b) - 1 + rv::Ntwo(-a, -b, rho);
        }
        else
        {
            denominator = std::sqrt(a * a - 2 * rho * a * b + b * b);
            rho1 = (rho * a - b) * rv::sgn(a) / denominator;
            rho2 = (rho * b - a) * rv::sgn(b) / denominator;
            result = rv::ND2(a, 0, rho1) + rv::ND2(b, 0, rho2) - (1 - rv::sgn(a) * rv::sgn(b)) / 4;
        }
        if (result < 0) result = 0;
    }
    return result;
}

double RandomVariableStatisticalFunctions::Ntwo(double a, double b, double rho)
{
    static double aij[4]={0.325303,
                          0.4211071,
                          0.1334425,
                          0.006374323};
    static double bij[4]={0.1337764,
                          0.6243247,
                          1.3425378,
                          2.2626645};
    double result = 0;
    for(int i = 0; i <= 3; ++i) 
    {
        for(int j = 0; j <= 3; ++j)
        {
            result += aij[i] * aij[j] * fxy(bij[i], bij[j], a, b, rho); 
        }
    }
    result *= std::sqrt(1 - rho * rho) / rv::PI;
    return result;
}


double RandomVariableStatisticalFunctions::fxy(double x, double y, double a, double b, double rho)
{
    double rho2 = std::sqrt(2 * (1 - rho * rho));
    double a_s = a / rho2;
    double b_s = b / rho2;
    double result;
    result = std::exp(a_s * (2 * x - a_s) + b_s * (2 * y - b_s) + 2 * rho * (x - a_s) * (y - b_s));
    return result;
}

double RandomVariableStatisticalFunctions::max(double a, double b)
{
    return (a > b) ? a : b;
}


double RandomVariableStatisticalFunctions::min(double a, double b)
{
    return (a > b) ? b : a;
}

// replicates Sgn as in visual basic, the signum of a real number
double RandomVariableStatisticalFunctions::sgn(double a)
{
    if ( a > 0)
        return 1.;
    else if (a < 0)
        return -1.;
    else
        return 0.;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  End
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
