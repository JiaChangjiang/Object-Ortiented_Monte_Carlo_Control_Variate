//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	utility.cpp
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

#include "utility.h"

#include <string>
#include <iostream>
#include <stdexcept>
#include <iomanip>
#include <cmath>
#include <limits>

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	PauseAndReturn()
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

int UtilityFunctions::PauseAndReturn()
{
	get_char_in_range("Enter q to quit: ", "q");
    return 0;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	PauseAndContinue()
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

void UtilityFunctions::PauseAndContinue()
{
    std::cout << "Press return to continue";
	std::cin.get();
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	bool UtilityFunctions::do_again()
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

bool UtilityFunctions::do_again()
{
	char input = '\0';
    
	std::cout << "Enter r to run again, anything else to quit: ";
    
	std::cin >> input;
    
    return input == 'r';
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	sign()
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

long UtilityFunctions::sign(double x)
{
	 if (x > 0) return 1;
	 if (x == 0) return 0;
	 return -1;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//XX     misc
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

bool UtilityFunctions::IsEven(const long x)
{
    return (x == 2*(x >> 1));
}

bool UtilityFunctions::IsOdd(const long x)
{
    return !IsEven(x);
}

bool UtilityFunctions::IsInteger(const double x, const double fuzz)
{
    return remainder(x, 1.) < fuzz;
}

bool UtilityFunctions::IsInteger(const double x)
{
    return x == (double)(long)x;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	ConvertString(),  specializations
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

template<> long UtilityFunctions::ConvertString(const std::string & ss, bool & succ)
{
	double x = ConvertString<double>(ss, succ);
	if (!succ) return long(x);
	
    succ = IsInteger(x);
	return long(x);
}

template<> bool UtilityFunctions::ConvertString(const std::string & s, bool & succ)
{
    succ = true;

	bool x = bool();
	std::istringstream i(s);
	if (i >> x) return x;  // values >> extracts depends on whether boolalpha is set
                           // eg 0/1 = false/true, or "false"/"true" = false/true
                           // since cannot rely upon >>,  do additional tests
    if (s == "true" || s == "y" || s == "1") return true;     // etc
    if (s == "false" || s == "n" || s == "0") return false;   // etc

    succ = false;
	return x;
}

template<> char UtilityFunctions::ConvertString(const std::string & s, bool & succ)
{
    succ = false;

    if (s.size() != 1) return '\0';

	char x = '\0';
	std::istringstream i(s);
	if (i >> x) succ = true;

	return x;
}

template<> char * UtilityFunctions::ConvertString(const std::string & ss, bool & succ)
{
    succ = false;

    long N = ss.length();

    char * name = new char[N + 1];
    for (long i = 0; i < N; ++i) name[i] = ss[i];
    name[N] = '\0';

    succ = true;
    return name;  // Passes over ownership of memory
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	ConvertToString()
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

template<> std::string UtilityFunctions::ConvertToString(const bool & x)
{
	return x ? "true" : "false";
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//XX     char functions
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

char UtilityFunctions::get_char_in_range(const std::string & prompt, const std::string & valid_chars)
{   /* promptes repeatedly from console */
	char x = '\0';
	while (!ut::check_char(x, valid_chars)) x = GetValue<char>(prompt);
	return x;
}

bool UtilityFunctions::check_char(const char & x, const std::string & valid_chars)
{
    return valid_chars.find(x) != std::string::npos;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	GetString()
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

std::string UtilityFunctions::GetString(std::istream & in)
{
	std::string text;
    if (!(in >> text)) throw std::runtime_error("ut::GetString:  Bad string");
    return text;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	Putters
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

void UtilityFunctions::OutputLine(std::ostream & out, const std::string & text)
{
	out << text << std::endl;
}

void UtilityFunctions::OutputLine(std::ostream & out, const std::string & text, const double & num)
{
	out << text << " " << num << std::endl;
}

void UtilityFunctions::OutputLine(std::ostream & out, const std::string & text, const double & num1, const double & num2)
{
	out << text << " " << num1 << " " << num2 << std::endl;
}

void UtilityFunctions::OutputLine(std::ostream & out, const std::string & text, const double & num1, const double & num2, const double & num3)
{
	out << text << " " << num1 << " " << num2 << " " << num3 << std::endl;
}

void UtilityFunctions::OutputLine(std::ostream & out, const double & num1, const double & num2)
{
	out << num1 << " " << num2 << std::endl;
}

void UtilityFunctions::OutputLine(std::ostream & out, const double & num1, const double & num2, const double & num3)
{
	out << num1 << " " << num2 << " " << num3 << std::endl;
}

void UtilityFunctions::OutputLine()
{
	std::cout << std::endl;
}

void UtilityFunctions::OutputLine(const std::string & text)
{
	std::cout << text << std::endl;
}

void UtilityFunctions::OutputLine(const std::string & text, const double & num)
{
	std::cout << text << " " << num << std::endl;
}

void UtilityFunctions::OutputLine(const std::string & t1, const std::string & t2)
{
	std::cout << t1 << " " << t2 << std::endl;
}

void UtilityFunctions::OutputLine(const std::string & text, const double & num, const long & prec)
{
	std::cout << std::setprecision(prec) << text << " " << num << std::endl;
}

///XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	OutputCounter();
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

void UtilityFunctions::OutputCounter(long i, long N, long OutputStepInterval)
{
	if (i == N) std::cout <<  "..." << i << std::endl;

	else if (0 == i%OutputStepInterval) std::cout <<  "..." << i;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	end
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
