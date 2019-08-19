//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//XX     RegressionOLS.cpp
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

#include "RegressionOLS.h"

#include<stdexcept>
#include<cmath>
#include <iostream>

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//XX
//XX	void ols_r(const ArrD & A, const VecD & b, VecD &  x)
//XX         Does OLS badly.  Solves Ax = b by solving A'Ax = A'b
//XX         A is m x n matrix.  b is m x 1. Results returned in x (n x 1)
//XX
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

void RegressionOLS::ols_r(const ArrD & A, const VecD & b, VecD &  x)
{
	long m = A.size();		// number of rows
	long n = A[0].size();	// number of columns
	            
    VecD p(n, 0.0);         // Returns the leading diagonal of C.D. of ATA
    ArrD ATA(n, p);         // A-transpose x A
    VecD ATb(n, 0.0);       // A-transpose x b
    
    for (long i = 0; i != n; ++i)               // Build the matrix ATA
    {
	    for (long j = 0; j != n;  ++j)
	    {
            ATA[i][j] = 0.0;
            for (long k = 0; k != m; ++k)
            {
                ATA[i][j] += A[k][i]*A[k][j];
            }
        }

     }

    for (long i = 0; i != n; ++i)                // Create the matrix ATb
    {
        ATb[i] = 0.0;
        for (long k = 0; k != m; ++k)
        {
            ATb[i] += A[k][i]*b[k];
        }
    }

    choldc(ATA, p);          // Cholesky decomposition of the positive definite matrix A
    
    cholsl(ATA, p, ATb, x);  // Results returned in x
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//XX
//XX         choldc(byref a() as double, n as long, byref p() as double
//XX         ex Numerical recipes.
//XX         Cholesky decompositon of the positive definite matrix A as A = L.LT
//XX         A is symmetric n x n.  Only the upper triangle of A is required.
//XX         Output is into the lower triangle of A,  except for the leading diagonal
//XX         which is put into p.  The upper triangle of A is not modified
//XX
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

void RegressionOLS::choldc(ArrD & A, VecD & p)
{
	long n = A.size();
	    
    for (long i = 0; i != n; ++i)
    {
        for (long j = i; j != n;  ++j)
        {
            double sum = A[i][j];
            for (long k = i - 1; k >= 0; --k)
            {
                sum -= A[i][k]*A[j][k];
            }
            
            if (i == j)
            {
                if (sum <= 0) throw std::runtime_error("choldc failed");
                
                p[i] = std::sqrt(sum);
            }
			else
			{
                A[j][i] = sum/p[i];
            }
		}
    }
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//XX
//XX    cholsl(const ArrD & A, const VecD & p, const VecD & b, VecD & x)
//XX	ex Numerical recipes
//XX    Solves the linear system Ax = b
//XX    A is positive definite n x n matrix,  assumed to be output by choldc
//XX    (p is the leading diagonal of A).  Only the lower triangle of A is read.
//XX    Results are returned in x.  A, b and p are not modified.
//XX
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

void RegressionOLS::cholsl(const ArrD & A, const VecD & p, const VecD & b, VecD & x)
{
	long n = A.size();
	
    for (long i = 0; i != n; ++i)      // Solves Ly = b. Puts y into x
    {
        double sum = b[i];
        for (long k = i - 1; k >= 0; --k)
        {
            sum -= A[i][k]*x[k];
        }
        x[i] = sum/p[i];
    }
    
    for (long i = n - 1; i >= 0; --i)     // Solves LTx = y.
    {
        double sum = x[i];
        for (long k = i + 1; k != n; ++k)
        {
            sum -= A[k][i]*x[k];
        }
        x[i] = sum/p[i];
    }
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//XX    Do a simple regression to compute optimal coeff for control variates
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

void RegressionOLS::SimpleRegression(double & a, double & b, const VecD & x, const VecD & y, double & rho)
{
	long m = x.size();
	
    double x_sum = 0.0;
	double y_sum = 0.0;
	double xy_sum = 0.0;
	double xx_sum = 0.0;
	double x_sq_sum = 0.0;
	double y_sq_sum = 0.0;
	
	for(long j = 0; j < m; ++j)
	{
		x_sum += x[j];
		y_sum += y[j];
		xy_sum += x[j]*y[j];
		xx_sum += x[j]*x[j];
	}
	
	double x_mean = x_sum/m;
	double y_mean = y_sum/m;
	
	for(long j = 0; j < m; ++j)
	{
		x_sq_sum += (x[j]-x_mean)*(x[j]-x_mean);
		y_sq_sum += (y[j]-y_mean)*(y[j]-y_mean);
	}
	
	double det = m*xx_sum - x_sum*x_sum;
	
	double x_std = std::sqrt(x_sq_sum/m-1);
	double y_std = std::sqrt(y_sq_sum/m-1);
	
    a = (xx_sum*y_sum - x_sum*xy_sum)/det;
	b = (-x_sum*y_sum + m*xy_sum)/det;
	
	rho = b * x_std/y_std;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  End
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

