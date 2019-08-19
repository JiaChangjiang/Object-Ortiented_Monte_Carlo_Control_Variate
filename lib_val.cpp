//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//XX   lib_val.cpp
//XX   Validators
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

#include "lib_val.h"

#include <string>
#include <sstream> 
#include <stdexcept> 
#include <cmath> 
#include <limits> 

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//XX          check_long_range
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

bool ValidationFunctions::check_long_range(const long x, const long l, const long u)
{
	if (u < l) return false;

	return l <= x && x <= u;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//XX     check_char
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

bool ValidationFunctions::check_char(std::string x, const std::string & valid_chars)
{
    if (x.size() != 1) return false;

    return valid_chars.find(x) != std::string::npos;
}

std::string ValidationFunctions::CharToString(const char * nm)
{
	return std::string(nm);
}

std::string ValidationFunctions::CharToString(const char nm)
{
	std::ostringstream o;
	o << nm;
    return o.str();
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//XX          StringToChar.  Care:  grabs memory.  Length issues
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

char * ValidationFunctions::StringToChar(std::string path)
{
    long num_chars = path.length();
    char * name = new char[num_chars+1];
    for (long i = 0; i < num_chars; ++i)
    {
        name[i] = path[i];
    }
    name[num_chars] = '\0';
    return name;
}

char ValidationFunctions::StringToCharacter(std::string path)
{
   	if (path.length() != 1) throw std::runtime_error("StringToCharacter failed");
    return path[0];
}

std::string ValidationFunctions::DoubleToString(double x)
{
	std::ostringstream o;
   	if (!(o << x)) throw std::runtime_error("DoubleToString failed");
	return o.str();
} 

double ValidationFunctions::StringToDouble(const std::string & s)
{
	std::istringstream i(s);
	double x;
	if (!(i >> x)) throw std::runtime_error("StringToDouble failed (" + s + ")");
	return x;
}

double ValidationFunctions::StringToDouble(const std::string & ss, bool & succ)
{
	succ = false;
	std::istringstream i(ss);
	double x;
	if (i >> x) succ = true;
	return x;
}

long ValidationFunctions::StringToLong(const std::string & s)
{
	std::istringstream i(s);
	long x;
	if (!(i >> x)) throw std::runtime_error("StringToLong failed (" + s + ")");
	return x;
}

long ValidationFunctions::StringToLong(const std::string & s, bool & success)
{
    success = false;
	std::istringstream i(s);
	double x;
	if (!(i >> x)) return long(x);

	long y = long(x);
	if (y == x) success = true;
	return y;
}

bool ValidationFunctions::StringToBool(const std::string & s)
{
    bool success;
    bool x = ValidationFunctions::StringToBool(s, success);

	if (!success) throw std::runtime_error("lv::StringToBool failed (" + s + ")");
	return x;
}

bool ValidationFunctions::StringToBool(const std::string & s, bool & success)
{
    success = true;

	bool x;
	std::istringstream i(s);
	if (i >> x) return x;

    if (s == "true" || s == "y") return true;
    if (s == "false" || s == "n") return false;

    success = false;
	return false;
}

std::string ValidationFunctions::BoolToString(bool x)
{
	return x ? "true" : "false";
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//XX     misc
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

bool ValidationFunctions::IsEven(const long x)
{
    return (x == 2*(x >> 1));
}

bool ValidationFunctions::IsOdd(const long x)
{
    return !IsEven(x);
}

bool ValidationFunctions::IsInteger(const double x, const double fuzz)
{
    return std::fabs(x - long(x + 0.5)) < fuzz;
}

bool ValidationFunctions::IsInteger(const double x)
{
    return IsInteger(x, 10.0*std::numeric_limits<double>::epsilon() );
}

std::string ValidationFunctions::join(const char * a, const char * b)
{
	std::ostringstream o;
   	o << a << b;
	return o.str();
}

std::string ValidationFunctions::join(const char * a, const std::string b)
{
	std::ostringstream o;
   	o << a << b;
	return o.str();
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//XX     end of file
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
