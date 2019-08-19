//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	InputterConsole.cpp
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

#include "InputterConsole.h"
#include "InputterCoded.h"
#include "MonitorManager.h"

#include "IOFactoryRegistration.h"
#include "CreateableBase.h"

#include "ParameterBase.h"
#include "ParameterS0.h"
#include "ParameterR.h"
#include "ParameterSigma.h"

#include "ParameterM.h"
#include "ParameterN.h"

#include "ParameterNu.h"

#include "utility.h"
#include "lib_val.h"

#include <map>
#include <string>
#include <sstream>
#include <iostream>

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  createable
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

template<> std::string ClassName<InputterConsole>()
{
    return "InputterConsole";
}

template<> std::string ClassID<InputterConsole>()
{
    return "s";
}

template<> std::string BaseClassName<InputterConsole>()
{
    return ClassName<InputterBase>();
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  register with factory
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

namespace
{
	IOFactoryRegistration<InputterBase, InputterConsole> RegisterInputterConsole;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	interface
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

InputterConsole::InputterConsole()
{
	mon_->OutputLine("Input is from console");
	
	inp_ = new InputterCoded;

    sd_map_ = inp_->GetParameters();
    sc_map_ = inp_->GetIdentifiers();
    ss_map_ = inp_->GetStrings();
    
    PromptForIDs();
    PromptForStrings();
    PromptForNumeric();
}

InputterConsole::~InputterConsole()
{
    delete inp_;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	prompters
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

void InputterConsole::PromptForIDs()
{
    if(sc_map_.empty()) return;
    ut::OutputLine("Please enter a character code for each identifier,  or return to accept the default value");
    
    char val;
    
    SC_it m_end = sc_map_.end();
    for (SC_it k = sc_map_.begin(); k != m_end; ++k) 
    {       
        std::ostringstream prompt;
        prompt << "  " << k->first << " (" << k->second << "):  ";
        if(GetChar(prompt.str(), val)) k->second = val;       
    }   
}

void InputterConsole::PromptForStrings()
{
    if(ss_map_.empty()) return;
    ut::OutputLine("Please enter values for each parameter,  or return to accept the default value");
    
    std::string val;

    SS_it m_end = ss_map_.end();
    for (SS_it k = ss_map_.begin(); k != m_end; ++k) 
    {
        std::ostringstream prompt;
        prompt << "  " << k->first << " (" << k->second << "):  ";
        if(GetString(prompt.str(), val)) k->second = val;       
    }   
}

void InputterConsole::PromptForNumeric()
{
    if(sd_map_.empty()) return;
    ut::OutputLine("Please enter values for each parameter,  or return to accept the default value");
    
    double val;

    SD_it m_end = sd_map_.end();
    for (SD_it k = sd_map_.begin(); k != m_end; ++k) 
    {
        std::ostringstream prompt;
        prompt << "  " << k->first << " (" << k->second << "):  ";
        if(GetDouble(prompt.str(), val)) k->second = val;       
    }   
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	getters
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

bool InputterConsole::GetChar(std::string prompt, char & val)
{
    std::string inp_strg;
    bool success = false;
    
    while (!success)
    {
        std::cout << prompt;
        std::getline(std::cin, inp_strg);
    
        if (inp_strg.empty()) return false;
    
        try
        {
            val = lv::StringToCharacter(inp_strg);
            success = true;
        }
        catch(...){}
    }
    
    return true;   
}
  
bool InputterConsole::GetDouble(std::string prompt, double & val)
{
    std::string inp_strg;
    bool success = false;
    
    while (!success)
    {
        std::cout << prompt;
        std::getline(std::cin, inp_strg);
    
        if (inp_strg.empty()) return false;
    
        try
        {
            val = lv::StringToDouble(inp_strg);
            success = true;
        }
        catch(...){}
    }
    
    return true;   
}

bool InputterConsole::GetString(std::string prompt, std::string & val)
{
    std::cout << prompt;
    std::getline(std::cin, val);

    if (val.empty()) return false;
    
    return true;   
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	end
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
