//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  SetUp.cpp
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

#include "SetUp.h"

#include "CreateableBase.h"

#include "ParameterMonitorPrintIO.h"
#include "ParameterMonitorPrintObj.h"
#include "ParameterMonitorPrintParas.h"
#include "ParameterInterval.h"

#include <stdexcept>
#include <string>

class MonitorBase;
class EnvironmentInputterBase;

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	template specialization definitions
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

template <> char SetUp::GetValue2(std::string str)
{
    return GetIdentifier(str);
}

template <> double SetUp::GetValue2(std::string str)
{
    return GetParameter(str);
}

template <> long SetUp::GetValue2(std::string str)
{
    return long(GetParameter(str));
}

template <> std::string SetUp::GetValue2(std::string str)
{
    return GetString(str);
}

template <> bool SetUp::GetValue2(std::string str)
{
    return GetBool(str);
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	structural
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

SetUp::SetUp()
{
    SetData();
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	private
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

double SetUp::GetParameter(const std::string & key)
{
     SD_it loc = sd_map_.find(key);
     if(loc == sd_map_.end())
     {
         throw std::runtime_error("SetUp::GetParameter" + key + " not found");
     }

     return loc->second;
}

char SetUp::GetIdentifier(const std::string & key)
{
     SC_it loc = sc_map_.find(key);
     if(loc == sc_map_.end())
     {
         throw std::runtime_error("SetUp::GetIdentifier,  " + key + " not found");
     }

     return loc->second;
}

std::string SetUp::GetString(const std::string & key)
{
     SS_it loc = ss_map_.find(key);
     if(loc == ss_map_.end())
     {
         throw std::runtime_error("SetUp::GetString,  " + key + " not found");
     }

     return loc->second;
}

bool SetUp::GetBool(const std::string & key)
{
     SB_it loc = sb_map_.find(key);
     if(loc == sb_map_.end())
     {
         throw std::runtime_error("SetUp::GetBool,  " + key + " not found");
     }

     return loc->second;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  SetData()
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

void SetUp::SetData()
{
	std::string MonitorType = "c";      // c for console
	std::string EnvironmentType = "d";  // d for default

	bool MonitorPrintIO = false;        // true to print IO object names
	bool MonitorPrintObj = false;      	// true to print IO application names
	bool MonitorPrintParas = false;     // true to print IO parameter values

    ss_map_.clear();
    sd_map_.clear();
    sb_map_.clear();

    ss_map_[ClassName<MonitorBase>()] = MonitorType;
    ss_map_[ClassName<EnvironmentInputterBase>()] = EnvironmentType;

    sd_map_[ParameterInterval().name] = 50000;
    
    sb_map_[ParameterMonitorPrintIO().name] = MonitorPrintIO;
    sb_map_[ParameterMonitorPrintObj().name] = MonitorPrintObj;
    sb_map_[ParameterMonitorPrintParas().name] = MonitorPrintParas;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  end of file
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

