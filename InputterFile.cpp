//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  InputterFile.cpp
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

#include "InputterFile.h"
#include "MonitorManager.h"

#include "ParameterInputFilePath.h"

#include "IOFactoryRegistration.h"
#include "ConfigurationManager.h"
#include "CreateableBase.h"

#include "utility.h"

#include <stdexcept>
#include <string>
#include <vector>

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  createable
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

template<> std::string ClassName<InputterFile>()
{
    return "InputterFile";
}

template<> std::string ClassID<InputterFile>()
{
    return "f";
}

template<> std::string BaseClassName<InputterFile>()
{
    return ClassName<InputterBase>();
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  register with factory
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

namespace
{
	IOFactoryRegistration<InputterBase, InputterFile> RegisterInputterFile;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	structural
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

InputterFile::InputterFile()
:   Data_Fetched_(false)
{
	input_file_path_ = ConfigurationManager::Instance().GetValue<std::string>(ParameterInputFilePath());
	
	mon_->OutputLine(ut::join("Input is from file ", input_file_path_));

    i_stream_.open(input_file_path_.c_str(), std::ios::in);
	if (!i_stream_.is_open())
	{
	    throw std::runtime_error("InputterFile:  cannot open input file");
	}

    FetchData();
	WriteMaps();
}

InputterFile::~InputterFile()
{
    i_stream_.close();
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  FetchData().  Very restrictive.  Up to the client to make the file correspond to this
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

void InputterFile::FetchData()
{
    if (Data_Fetched_) return;
	
    sc_map_.clear();
    sd_map_.clear();
    ss_map_.clear();

	sd_map_.insert(FetchPair<double>());        // S0
    sd_map_.insert(FetchPair<double>());        // X 
    sd_map_.insert(FetchPair<double>());        // r
    sd_map_.insert(FetchPair<double>());        // sigma
    sd_map_.insert(FetchPair<double>());        // rho
    
    sd_map_.insert(FetchPair<double>());        // M
    sd_map_.insert(FetchPair<double>());        // N
    sd_map_.insert(FetchPair<double>());        // T
    
	sd_map_.insert(FetchPair<double>());        // y0
	
    sd_map_.insert(FetchPair<double>());        // Hull White sigma
    sd_map_.insert(FetchPair<double>());        // Hull White m
    sd_map_.insert(FetchPair<double>()); 		// Hull White mu
    
    sd_map_.insert(FetchPair<double>()); 		// k
    sd_map_.insert(FetchPair<double>()); 		// theta
    
    sd_map_.insert(FetchPair<double>()); 		// alpha
    sd_map_.insert(FetchPair<double>()); 		// beta

    ss_map_.insert(FetchPair<std::string>());   // Deterministic Volatility Type
    ss_map_.insert(FetchPair<std::string>());   // Stochastic volatility Type
    ss_map_.insert(FetchPair<std::string>());   // Option Type
	ss_map_.insert(FetchPair<std::string>());   // MonteCarlo Type

	Data_Fetched_ = true;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	utilities
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

void InputterFile::WriteMaps()
{
    ut::OutputLine("The following data has been input from file:");
	WriteSDmap();
    ut::OutputLine();
    WriteSCmap();
    ut::OutputLine();
    WriteSSmap();
    ut::OutputLine();
}

void InputterFile::WriteSDmap()
{
     SD_it it_end = sd_map_.end();
     for(SD_it it = sd_map_.begin(); it != it_end;  ++it )
     {
         ut::OutputLine(it->first, it->second);
     }
}

void InputterFile::WriteSCmap()
{
     SC_it it_end = sc_map_.end();
     for(SC_it it = sc_map_.begin(); it != it_end;  ++it )
     {
         ut::OutputLine(it->first, char(it->second));
     }
}

void InputterFile::WriteSSmap()
{
     SS_it it_end = ss_map_.end();
     for(SS_it it = ss_map_.begin(); it != it_end;  ++it )
     {
         ut::OutputLine(it->first, it->second);
     }
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  end of file
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

