//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  OutputterFile.cpp
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

#include "OutputterFile.h"
#include "MonitorManager.h"

#include "ParameterOutputFilePath.h"

#include "IOFactoryRegistration.h"
#include "ConfigurationManager.h"
#include "CreateableBase.h"

#include "utility.h"

#include <string>
#include <fstream>

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  createable
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

template<> std::string ClassName<OutputterFile>()
{
    return "OutputterFile";
}

template<> std::string ClassID<OutputterFile>()
{
    return "f";
}

template<> std::string BaseClassName<OutputterFile>()
{
    return "OutputterBase";
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  register with factory
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

namespace
{
	IOFactoryRegistration<OutputterBase, OutputterFile> RegisterOutputterFile;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  structural
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

OutputterFile::OutputterFile()
:   precision_(6)    	//set precision,  as you wish
,   width_(15) 			//set width,  as you wish
{
	output_file_path_ = ConfigurationManager::Instance().GetValue<std::string>(ParameterOutputFilePath());

	mon_->OutputLine(ut::join("Output is to file ", output_file_path_));

    //over-write file if exists
    o_stream_.open(output_file_path_.c_str(), std::ios::out | std::ios::trunc);  // a.
    
	o_stream_.precision(precision_);                             		// b.
	o_stream_.width(width_);                                     		// c.
}

OutputterFile::~OutputterFile()
{
    o_stream_.close();                                                  // d.
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	interface
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

void OutputterFile::DoOutput(SD_map & map)
{
    ut::OutputLine(o_stream_, "");

    SD_it m_end = map.end();
    for (SD_it k = map.begin(); k != m_end; ++k) // iterators iterate
    {
        o_stream_ << k->first << ": " << k->second << '\n';
    }
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  end of file
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

