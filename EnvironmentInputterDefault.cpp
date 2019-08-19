//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	EnvironmentInputterDefault.cpp
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

#include "EnvironmentInputterDefault.h"

#include "ConfigurationFactoryRegistration.h"
#include "CreateableBase.h"

#include "ParameterInterval.h"
#include "ParameterOutputFilePath.h"
#include "ParameterInputFilePath.h"

#include <stdexcept>

class InputterBase;
class OutputterBase;

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  createable
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

template<> std::string ClassName<EnvironmentInputterDefault>()
{
    static std::string name = "EnvironmentInputterDefault";
    return name;
}

template<> std::string ClassID<EnvironmentInputterDefault>()
{
    static std::string name = "d";
    return name;
}

template<> std::string BaseClassName<EnvironmentInputterDefault>()
{
    return ClassName<EnvironmentInputterBase>();
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  register with factory
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

namespace
{
	ConfigurationFactoryRegistration<EnvironmentInputterBase, EnvironmentInputterDefault> RegisterEID;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	structural
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

EnvironmentInputterDefault::EnvironmentInputterDefault()
{
    sd_map_.clear();
   	sc_map_.clear();
    ss_map_.clear();

   	ss_map_[ParameterInputFilePath().name] = "Input_data.txt";	 	// relative path
   	ss_map_[ParameterOutputFilePath().name] = "Output_data.txt";	// relative path

   	ss_map_[ClassName<InputterBase>()] = "f";     					// c for code, f for file, s for console
   	ss_map_[ClassName<OutputterBase>()] = "c";    					// c for console, f for file
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	end
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

