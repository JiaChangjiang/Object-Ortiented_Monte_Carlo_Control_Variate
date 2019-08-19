//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  lib_val.h
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
#ifndef lib_valH
#define lib_valH
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

#include <string>

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  namespace ValidationFunctions
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

namespace ValidationFunctions
{
	bool check_long_range(const long x, const long l, const long u);

    bool check_char(std::string, const std::string &);
    
	char * StringToChar(std::string path);
    char StringToCharacter(std::string path);
	std::string CharToString(const char * nm);
	std::string CharToString(const char nm);

	std::string DoubleToString(double x);

	double StringToDouble(const std::string & s);
 	double StringToDouble(const std::string & s, bool &);

	long StringToLong(const std::string & s);
	long StringToLong(const std::string & s, bool &);

    bool StringToBool(const std::string & s);
    bool StringToBool(const std::string & s, bool &);
    
    std::string BoolToString(bool);
    
    bool IsEven(const long x);
    bool IsOdd(const long x);
    bool IsInteger(const double x, const double fuzz);
    bool IsInteger(const double x);

    std::string join(const char *, const char *);
	std::string join(const char *, const std::string);
}

namespace lv = ValidationFunctions;    //alias

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
#endif
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  End
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
