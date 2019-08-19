//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  utility.h
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
#ifndef utilityH
#define utilityH
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <stdexcept>

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  namespace UtilityFunctions
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

namespace UtilityFunctions
{
	int PauseAndReturn();
	void PauseAndContinue();
	bool do_again();

	long sign(double);
    bool IsEven(const long);
    bool IsOdd(const long);
    bool IsInteger(const double x);
    bool IsInteger(const double x, const double fuzz);

    //XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
    //	char stuff
    //XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

	char get_char_in_range(const std::string &, const std::string &);
	bool check_char(const char &, const std::string &);

    //XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
    //	OutputLine()
    //XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

	void OutputLine(std::ostream &, const std::string &);
	void OutputLine(std::ostream &, const std::string &, const double &);
	void OutputLine(std::ostream &, const std::string &, const double &, const double &);
	void OutputLine(std::ostream &, const std::string &, const double &, const double &, const double &);
	void OutputLine(std::ostream &, const double &, const double &);
	void OutputLine(std::ostream &, const double &, const double &, const double &);
	void OutputLine();
	void OutputLine(const std::string &);
	void OutputLine(const std::string &, const std::string &);
	void OutputLine(const std::string &, const double &);
	void OutputLine(const std::string &, const double &, const long &);

    //XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
    //	OutputNumeric()
    //XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

	template<typename T> void OutputNumeric(const T & t, long N)
	    {std::cout << std::setprecision(N) << t << std::endl;}

	template<typename S, typename T> void OutputNumeric(const S & s, const T & t, long N)
	    {std::cout << std::setprecision(N) << s << ":  " << t << std::endl;}

    //XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
    //	ReachedHere()
    //XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
    
	template<typename T> void ReachedHere(std::ostream & out, const T & t)
        { out <<  "Reached here:  " << t << std::endl;}

	template<typename T> void ReachedHere(const T & t)
        {ReachedHere<T>(std::cout, t);}

    //XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
    //	ConvertString()
    //XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

    template<typename S> S ConvertString(const std::string & ss, bool & succ)
    {
    	succ = false;
    	std::istringstream i(ss);
    	S s = S();
    	if (i >> s) succ = true;
    	return s;
    }

    template<typename S> S ConvertString(const std::string & ss)
    {
        bool success;
        S s = ConvertString<S>(ss, success);

    	if (!success) throw std::runtime_error("ConvertString failed)");
    	return s;
    }

    template<> long ConvertString(const std::string & ss, bool & succ);
    template<> bool ConvertString(const std::string & ss, bool & succ);
    template<> char ConvertString(const std::string & ss, bool & succ);
    template<> char * ConvertString(const std::string & ss, bool & succ);

    //XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
    //	ConvertToString()
    //XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

    template<typename T> std::string ConvertToString(const T & t)
    {
    	std::ostringstream o;
       	if (!(o << t)) throw std::runtime_error("ConvertToString failed");
    	return o.str();
    }

    template<> std::string ConvertToString(const bool &);

    //XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
    //	GetString()
    //XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

    std::string GetString(std::istream & in);

    template<typename T> std::string GetString(std::istream & in, const T & t)
    {
        std::cout << std::endl << t << ":  ";
    	return GetString(in);
    }

    template<typename T> std::string GetString(std::iostream & strm, const T & t)
    {
        strm << std::endl << t << ":  ";
    	return GetString(strm);
    }

    template<typename T> std::string GetString(const T & t)
    {
        std::cout << std::endl << t << ":  ";
    	return GetString(std::cin);
    }

    //XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
    //	GetValue()
    //XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

    template<typename S> S GetValue(std::istream & in, bool & success)
    {   /*  Assumes that *cannot* prompt repeatedly  */
    	success = false;
    	std::string input = GetString(in);
     	return ConvertString<S>(input, success);
    }

    template<typename S> S GetValue(std::istream & in)
    {   /*  Assumes that *cannot* prompt repeatedly  */
    	bool success = false;
     	S s = GetValue<S>(in, success);
     	if (!success) throw std::runtime_error("GetValue failed");
    	return s;
    }

    template<typename S, typename T> S GetValue(std::iostream & strm, const T & t)
    {   /*  Assumes that it is appropriate to prompt repeatedly  */
    	bool success = false;
    	S s = S();

    	while (!success)
    	{
    		std::string input = GetString(strm, t);
     		s = ConvertString<S>(input, success);
    	}

    	return s;
    }
    
    template<typename S, typename T> S GetValue(std::istream & in, const T & t)
    {   /*  Assumes that it is appropriate to prompt repeatedly  */
    	bool success = false;
    	S s = S();

    	while (!success)
    	{
    		std::string input = GetString(in, t);
     		s = ConvertString<S>(input, success);
    	}

    	return s;
    }
    
    template<typename S, typename T> S GetValue(const T & t)
    {   /*  Prompts repeatedly from console  */
    	bool success = false;
    	S s = S();

    	while (!success)
    	{
   		    std::string input = GetString(t);
     		s = ConvertString<S>(input, success);
    	}

    	return s;
    }

    //XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
    //	Range stuff
    //XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

	template<typename T, typename S> bool CheckRange(const T & t, const S & l, const S & u)
    {
    	if (u < l) throw std::runtime_error("GetInRange: bad range");
    	return l <= t && t <= u;
    }

	template<typename T> bool CheckPositive(const T & t)
    {
    	return t <= 0;
    }

	template<typename T, typename S> T GetInRange(const S & prompt, T l, T u)
    {   /*  Prompts repeatedly from console  */
    	bool success = false;
    	T t = T();

    	while (!success)
    	{
    		t = GetValue<T>(prompt);
    		success = CheckRange(t, l, u);
    		if (!success) OutputLine("Not in range");
    	}
        return t;
    }
	
	template<typename T, typename S> T GetPositive(const S & prompt)
    {   /*  Prompts repeatedly from console  */
    	bool success = false;
    	T t = T();

    	while (!success)
    	{
    		t = GetValue<T>(prompt);
     		success = CheckPositive(t);
    		if (!success) OutputLine("Not strictly positive");
    	}

    	return t;
    }

    //XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
    //	Misc.
    //XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

    void OutputCounter(long i, long N, long OutputStepInterval);

	template<typename T, typename S> std::string join(const T & t, const S & s)
    {
    	std::ostringstream o;
       	o << t << s;
    	return o.str();  // returns a copy of the stream contents.
    }
}

namespace ut = UtilityFunctions;    //alias

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
#endif
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  End
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
