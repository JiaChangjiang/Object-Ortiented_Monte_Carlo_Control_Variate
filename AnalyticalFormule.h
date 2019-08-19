//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	AnalyticalFormulae.h
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
#ifndef AnalyticalFormulaeH
#define AnalyticalFormulaeH
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	name space AnalyticalFormulae
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

namespace AnalyticalFormulae
{
	double Euro_put_HullWhite_NewMethod
			(double S, double r, double X, double T, double mu, double m, double hull_white_sig, double y0);
	double Euro_put_HullWhite_MethodI
			(double S, double r, double X, double T, double mu, double m, double hull_white_sig, double y0);
	double Euro_put_HullWhite_MethodII
			(double S, double r, double X, double T, double mu, double m, double hull_white_sig, double y0);

	double Euro_put_Heston_NewMethod
			(double S, double r, double X, double T, double y0, double k, double theta);
	double Euro_put_Heston_MethodI
			(double S, double r, double X, double T, double y0, double k, double theta);
	double Euro_put_Heston_MethodII
			(double S, double r, double X, double T, double y0, double k, double theta);

	double Euro_put_Steinstein_NewMethod
			(double S, double r, double X, double T, double y0, double alpha, double beta);
			
	double Euro_call_HullWhite_NewMethod
			(double S, double r, double X, double T, double mu, double m, double hull_white_sig, double y0);
	double Euro_call_HullWhite_MethodI
			(double S, double r, double X, double T, double mu, double m, double hull_white_sig, double y0);
	double Euro_call_HullWhite_MethodII
			(double S, double r, double X, double T, double mu, double m, double hull_white_sig, double y0);

	double Euro_call_Heston_NewMethod
			(double S, double r, double X, double T, double y0, double k, double theta);
	double Euro_call_Heston_MethodI
			(double S, double r, double X, double T, double y0, double k, double theta);
	double Euro_call_Heston_MethodII
			(double S, double r, double X, double T, double y0, double k, double theta);
			
	double Euro_call_Steinstein_NewMethod
			(double S, double r, double X, double T, double y0, double alpha, double beta);

	double Asian_call_HullWhite_NewMethod
			(double S, double r, double X, double T, double y0, double mu, double hull_white_sig, double m);
}

namespace af = AnalyticalFormulae;    //alias

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
#endif
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	End
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
