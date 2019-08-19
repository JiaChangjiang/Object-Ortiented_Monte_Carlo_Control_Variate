//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  SetUp.h
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
#ifndef SetUpH
#define SetUpH
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

#include <string>
#include <map>
#include <stdexcept>

class ParameterBase;

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  class SetUp
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

class SetUp
{
    public:
		SetUp();

        template <typename T, typename S> void GetValue(const T & t)
        {
            t.value = GetValue2<S>(t.name);
        }

    private:
        typedef std::map<std::string, double> SD_map;
        typedef std::map<std::string, double>::iterator SD_it;

        typedef std::map<std::string, char> SC_map;
        typedef std::map<std::string, char>::iterator SC_it;

        typedef std::map<std::string, std::string> SS_map;
        typedef std::map<std::string, std::string>::iterator SS_it;

        typedef std::map<std::string, bool> SB_map;
        typedef std::map<std::string, bool>::iterator SB_it;

        template <typename S> S GetValue2(std::string str)
		{
		    throw std::runtime_error("SetUp.  Right map not found:  " + str);
		}

        double GetParameter(const std::string &);
        char GetIdentifier(const std::string &);
        std::string GetString(const std::string &);
        bool GetBool(const std::string &);

        SD_map sd_map_;  // doubles/longs map
        SC_map sc_map_;  // char map
        SS_map ss_map_;  // std::strings map
        SB_map sb_map_;  // bools map

        void SetData();
};

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  template specializations.  Declare here (namespace level),  give defn in .cpp
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

template <> char SetUp::GetValue2(std::string str);
template <> double SetUp::GetValue2(std::string str);
template <> long SetUp::GetValue2(std::string str);
template <> std::string SetUp::GetValue2(std::string str);
template <> bool SetUp::GetValue2(std::string str);

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
#endif
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  end of file
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

