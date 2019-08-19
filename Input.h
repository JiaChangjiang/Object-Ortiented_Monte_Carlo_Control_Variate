//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  Input.h
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
#ifndef InputH
#define InputH
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  class Input
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
#include <vector>
#include <string>

class Input
{
public:
	Input();
	//get general parameters
	long GetM() const { return M_; }
	long GetN() const { return N_; }
	double GetX() const { return X_; }
	double GetT() const { return T_; }
	double Getr() const { return r_; }

	//get hull white parameters
	double GetHullWhite_S0() const { return hullwhite_S_0_; }
	double GetHullWhite_r() const { return hullwhite_r_; }
	double GetHullWhite_X() const { return hullwhite_X_; }
	double GetHullWhite_T() const { return hullwhite_T_; }
	double GetHullWhite_m() const { return hullwhite_m_; }

	double GetHullWhite_miu() const { return hullwhite_miu_; }
	double GetHullWhite_sig() const { return hullwhite_sig_; }
	double GetHullWhite_rho() const { return hullwhite_rho_; }
	double GetHullWhite_Y0() const { return hullwhite_Y0_; }

	std::vector<double> & GetChange_HullWhite_S0() { return change_hullwhite_S_0_; }
	std::vector<double> & GetChange_HullWhite_rho() { return change_hullwhite_rho_; }
	std::vector<double> & GetChange_HullWhite_m() { return change_hullwhite_m_; }
	//get heston parameters
	double GetHeston_S0() const { return heston_S_0_; }
	double GetHeston_r() const { return heston_r_; }
	double GetHeston_X() const { return heston_X_; }
	double GetHeston_T() const { return heston_T_; }
	double GetHeston_m() const { return heston_m_; }

	double GetHeston_k() const { return heston_k_; }
	double GetHeston_theta() const { return heston_theta_; }
	double GetHeston_sig() const { return heston_sig_; }
	double GetHeston_rho() const { return heston_rho_; }
	double GetHeston_Y0() const { return heston_Y0_; }

	std::vector<double> & GetChange_Heston_S0() { return change_heston_S_0_; }
	std::vector<double> & GetChange_Heston_rho() { return change_heston_rho_; }
	std::vector<double> & GetChange_Heston_sig() { return change_heston_sig_; }
	std::vector<double> & GetChange_Heston_T() { return change_heston_T_; }
	std::vector<double> & GetChange_Heston_k() { return change_heston_k_; }
	//get stein stein parameters
	double GetSteinStein_S0() const { return heston_S_0_; }
	double GetSteinStein_r() const { return heston_r_; }
	double GetSteinStein_X() const { return heston_X_; }
	double GetSteinStein_T() const { return heston_T_; }
	double GetSteinStein_m() const { return heston_m_; }

	double GetSteinStein_alpha()const { return steinstein_alpha_; }
	double GetSteinStein_beta() const { return steinstein_beta_; }
	double GetSteinStein_sig() const { return steinstein_sig_; }
	double GetSteinStein_rho() const { return steinstein_rho_; }
	double GetSteinStein_Y0() const { return steinstein_Y0_; }

	std::vector<double> & GetChange_SteinStein_rho() { return change_steinstein_rho_; }
	std::vector<double> & GetChange_SteinStein_sig() { return change_steinstein_sig_; }
	std::vector<double> & GetChange_SteinStein_T() { return change_steinstein_T_; }
	std::vector<double> & GetChange_SteinStein_alpha() { return change_steinstein_alpha_; }

	//set general parameters
	void SetM(long M) { M_ = M; }
	void SetN(long N) { N_ = N; }

	//get hull white parameters
	void SetHullWhite_S0(double s_0) { hullwhite_S_0_ = s_0; }
	void SetHullWhite_r(double r) { hullwhite_r_ = r; }
	void SetHullWhite_X(double X) { hullwhite_X_ = X; }
	void SetHullWhite_T(double T) { hullwhite_T_ = T; }
	void SetHullWhite_m(double m) { hullwhite_m_ = m; }

	void SetHullWhite_miu(double miu) { hullwhite_miu_ = miu; }
	void SetHullWhite_sig(double sig) { hullwhite_sig_ = sig; }
	void SetHullWhite_rho(double rho) { hullwhite_rho_ = rho; }
	void SetHullWhite_Y0(double Y0)  { hullwhite_Y0_ = Y0; }
	//get heston parameters
	void SetHeston_S0(double s_0) { heston_S_0_ = s_0; }
	void SetHeston_r(double r) { heston_r_ = r; }
	void SetHeston_X(double X) { heston_X_ = X; }
	void SetHeston_T(double T) { heston_T_ = T; }
	void SetHeston_m(double m) { heston_m_ = m; }

	void SetHeston_k(double k) { heston_k_ = k; }
	void SetHeston_theta(double theta) { heston_theta_ = theta; }
	void SetHeston_sig(double sig) { heston_sig_ = sig; }
	void SetHeston_rho(double rho) { heston_rho_ = rho; }
	void SetHeston_Y0(double Y0) { heston_Y0_=Y0; }
	//get stein stein parameters
	void SetSteinStein_S0(double s_0) { steinstein_S_0_ = s_0; }
	void SetSteinStein_r(double r) { steinstein_r_ = r; }
	void SetSteinStein_X(double X) { steinstein_X_ = X; }
	void SetSteinStein_T(double T) { steinstein_T_ = T; }
	void SetSteinStein_m(double m) { steinstein_m_ = m; }

	void SetSteinStein_alpha(double alpha) { steinstein_alpha_ = alpha; }
	void SetSteinStein_beta(double beta) { steinstein_beta_ = beta; }
	void SetSteinStein_sig(double sig) { steinstein_sig_ = sig; }
	void SetSteinStein_rho(double rho) { steinstein_rho_ = rho; }
	void SetSteinStein_Y0(double Y0) { steinstein_Y0_ = Y0; }

	std::string GetMethodType() { return methodtype; }
	std::string GetProcessType() { return processtype; }

private: 
	//iteration parameters
	long M_;     // number of sample paths
	long N_;     // number of time steps 
	double X_;
	double T_;
	double r_;

	//hull white parameters
	double hullwhite_S_0_;
	double hullwhite_r_;
	double hullwhite_X_;
	double hullwhite_T_;
	double hullwhite_m_;

	double hullwhite_rho_;
	double hullwhite_miu_;
	double hullwhite_sig_;
	double hullwhite_Y0_;

	std::vector<double> change_hullwhite_S_0_;
	std::vector<double> change_hullwhite_rho_;
	std::vector<double> change_hullwhite_m_;
	//heston paramete
	double heston_S_0_;
	double heston_r_;
	double heston_X_;
	double heston_T_;
	double heston_m_;

	double heston_k_;
	double heston_theta_;
	double heston_sig_;
	double heston_rho_;
	double heston_Y0_;

	std::vector<double> change_heston_S_0_;
	std::vector<double> change_heston_rho_;
	std::vector<double> change_heston_sig_;
	std::vector<double> change_heston_T_;
	std::vector<double> change_heston_k_;
	//stein stein parameters
	double steinstein_S_0_;
	double steinstein_r_;
	double steinstein_X_;
	double steinstein_T_;
	double steinstein_m_;

	double steinstein_alpha_;
	double steinstein_beta_;
	double steinstein_sig_;
	double steinstein_rho_;
	double steinstein_Y0_;

	std::vector<double> change_steinstein_rho_;
	std::vector<double> change_steinstein_sig_;
	std::vector<double> change_steinstein_T_;
	std::vector<double> change_steinstein_alpha_;

	//parameter setting
	std::string optiontype;
	std::string methodtype;
	std::string processtype;

	std::vector<std::string> Process;
	std::vector<std::string> Methods;
	std::vector<std::string> Parameter;
};
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
#endif
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  End
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
