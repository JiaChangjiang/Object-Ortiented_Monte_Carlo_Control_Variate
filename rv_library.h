//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//   rv_library.h
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
#ifndef rv_libraryH
#define rv_libraryH
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

#include <vector>

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  namespace RandomVariableStatisticalFunctions
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

namespace RandomVariableStatisticalFunctions
{
    //XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
    //  typedefs
    //XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

		typedef std::vector<double> VecD;
		typedef std::vector<VecD> ArrD;

    //XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
    //  Prototypes
    //XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

		double n_probs(double);				//Normal density function
		double ran0();						//ex Numerical recipes (slightly modified)
	    double my_ran2();           		//ex Numerical recipes,  modifed to use static
	    double GetNormalVariate();  		//Polar rejection
	    double N(double z);         		//Cumulative normal distribution function.  Not accurate
	    double ltqnorm(double p, int err);  //Inverse cumulatice normal dist function.  Accurate to 10^-9
	    double cndev(double u);     		//Inverse cumulatice normal dist function.
	    double Cody_erf(double X, long jint);
	    double normal_cdf(double X);
	    double Ex(const double lambda);             //Simulates from a Exponential distribution, mean lambda
		long Poisson_jumps(const double lambda);	//Simulates from a Poisson distribution, parameter lambda
        long Poisson_jumps_inv(const double lambda, const double u); //inverse a Poisson distribution
        long Poisson_jumps2(const double lambda);   //Simulate a Poisson variate using rejection method

	    void sobseq(std::vector<double> &);
        void Get_Brownian_bridge(std::vector<double> &, const double, const long, const long, const long);
	    double Get_one_Wiener_bridge_step(const double, const double, const double, const double);
        void Get_Brownian_bridge(std::vector<double> &, const double dt, const std::vector<double> &);

        double normpdf(const double x, const double mu, const double sigma);    //normal density for first passage time        
        double hittingDist(const double a, const double b, const double t, const double mu, const double sigma); //hitting time density
		double ranHittingDist
            (const double a, const double b, const double drift, const double sigma);  //Simulate from Hittng Distribution (IG with negative drift)   
        
		double ranInverseGaussian(double nu, double lambda);                                                                                                                   
        double InverseGaussianpdf(const double x, const double a, const double mu);      //Inverse Gaussian Density
        double InverseGaussiancdf(const double x, const double lambda, const double nu);      //Inverse Gaussian CDF

	    double maxd(double a, double b);
	    double mind(double a, double b);

		double NC_Chi_squ_cdf(const double x, double const v, double const lam);
		double Chi_squ_pdf(const double x, const double a);
        double Chi_squ_cdf(const double x, const double a);
        double Chi_squ_dist_inv(const double u, const double a);
        
        double gammln(const double xx);
		double gammp(const double a, const double x);
		double invgammp(const double p, const double x);
		double gser(const double a, const double x);
		double gcf(const double a, const double x);

        long rand_bessel(const double x, const double xnu);
        double besseli(const double x, const double xnu);
		void bessik(const double x, const double xnu, double &ri, double &rk, double &rip, double &rkp);
		void beschb(const double x, double &gam1, double &gam2, double &gampl, double &gammi);
		double chebev(const double  a, const double b, const double * c, const long m, const double x);

		double Brownian_bridge_hitting_time_dist
			(const double, const double, const double, const double, const double,const double);
			
		double Brownian_bridge_maximum_dist
			(const double, const double, const double, const double, const double);

		double Brownian_bridge_minimum_dist
			(const double, const double, const double, const double, const double);

		double Brownian_bridge_maximum_draw
			(const double, const double, const double, const double, const double);
					
		double Wiener_bridge_maximum_draw
			(const double, const double, const double, const double);
					
		double Brownian_bridge_minimum_draw
			(const double, const double, const double, const double, const double);
					
		double Wiener_bridge_minimum_draw
			(const double, const double, const double, const double);
			
		double rand_gamma(double);			//simulation of gamma variates
		double rand_gamma_small(double);
		double rand_gamma_large(double);
		double rand_gamma(double, double); //simulation of gamma variates. Rejection method

		double rand_student(double);		//siulation of student t with V dof
		double rand_ig(double, double);		//simulation of inverse Gaussian variate
							
		double student_3_density(double);	//density of student distribution with 3 dof
		double student_3_dist(double);		//dist fn of student distribution with 3 dof
		double student_3_dist_inv(double);	//numerical approx to inverse t3 fn

		double student_5_density(double);	//density of student distribution with 5 dof
		double student_5_dist(double);		//dist fn of student distribution with 5 dof
		double student_5_dist_inv(double);	//numerical approx to inverse t5 fn

        double ND2(double a, double b, double rho);  //cumulative bivariate normal distribution function
        double Ntwo(double a, double b, double rho);
		double fxy(double x, double y, double a, double b, double rho);
        double max(double a, double b);
        double min(double a, double b);
        double sgn(double a);
        					
    //XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
    //  Handy constants
    //XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

	    const long TWO = 2;                 //Constants
	    const long ONE = 1;
	    const long ZERO = 0;

    //XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
    //  Numerical constants
    //XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

	    const double PI = 3.1415926535897932384626433832795;        // PI
	    const double PI_BY_TWO = 1.57079632679489661923;            // PI/2
	    const double PI_BY_FOUR = 0.785398163397448309616;          // PI/4
	    const double TWO_PI= 6.28318530717959;       		        // 2*PI
	    const double ROOT_TWO_PI = 2.506628274631;    		        // sqrt(2*PI)
	    const double ROOT_PI_INVERSE = 0.564189583547756286948;     // 1/sqrt(PI)
	    const double TWO_ROOT_PI_INVERSE = 1.12837916709551257390;  // 2/sqrt(PI)
	    const double PI_INVERSE = 0.318309886183790671538;          // 1/PI
	    const double TWO_PI_INVERSE = 0.636619772367581343076;      // 2/PI

	    const double ROOT_TWO = 1.41421356237309504880;		        // sqrt(2)
	    const double ROOT_TWO_INVERSE = 0.707106781186547524401;	// 1/sqrt(2)
	    const double ROOT_THREE = 1.73205080756888;			        // sqrt(3)
	    const double ROOT_THREE_PI_BY_TWO = 2.7206699046;	        // sqrt(3)*PI/2
	    const double ROOT_FIVE = 2.23606797749979;			        // sqrt(5)
	    
	    const double LN_TWO = 0.693147180559945309417;			        // ln(2)
	    const double LN_TEN = 2.30258509299404568402;			        // ln(10)
	    const double EXP_1 = 2.71828182845904523536;	            // exp(1)
}

namespace rv = RandomVariableStatisticalFunctions;    //alias

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
#endif
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  End
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

