//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	EnvironmentManager.cpp
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

#include "EnvironmentManager.h"

#include "EnvironmentInputterBase.h"
#include "ConfigurationFactory.h"

#include <stdexcept>

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	template specialization definitions
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

template <> char EnvironmentManager::GetValue2(std::string str)
{
    return GetValue3(sc_map_, str);
}

template <> double EnvironmentManager::GetValue2(std::string str)
{
    return GetValue3(sd_map_, str);
}

template <> long EnvironmentManager::GetValue2(std::string str)
{
    return long(GetValue3(sd_map_, str));
}

template <> std::string EnvironmentManager::GetValue2(std::string str)
{
    return GetValue3(ss_map_, str);
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	structural
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

EnvironmentManager::EnvironmentManager()
:	env_(0)
{
	env_ = ConfigurationFactory<EnvironmentInputterBase>::Instance().CreateObject();

    FetchData();
}

EnvironmentManager::~EnvironmentManager()
{
    delete env_;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	private
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

template <typename S> 
S EnvironmentManager::GetValue3(std::map<std::string, S> map, std::string & key)
{
     typename std::map<std::string, S>::iterator S_it = map.find(key);
     if(S_it == map.end())
     {
         throw std::runtime_error("EnvironmentManager::GetValue3" + key + " not found");
     }

     return S_it->second;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	FetchData()
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

void EnvironmentManager::FetchData()
{
    sd_map_ = env_->GetParameters();
    sc_map_ = env_->GetIdentifiers();
    ss_map_ = env_->GetStrings();
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	end
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

