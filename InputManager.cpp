//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	InputManager.cpp
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

#include "InputManager.h"

#include "IOFactory.h"
#include "InputterBase.h"

#include "utility.h"

#include <map>
#include <string>
#include <stdexcept>

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	template specialization definitions
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

template <> char InputManager::GetValue2(std::string str)
{
    return GetValue3(sc_map_, str);
}

template <> double InputManager::GetValue2(std::string str)
{
    return GetValue3(sd_map_, str);
}

template <> long InputManager::GetValue2(std::string str)
{
    return long(GetValue3(sd_map_, str));
}

template <> std::string InputManager::GetValue2(std::string str)
{
    return GetValue3(ss_map_, str);
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	structural
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

InputManager::InputManager()
:	inp_(0)
{
    inp_ = IOFactory<InputterBase>::Instance().CreateObject();

    FetchData();
}

InputManager::~InputManager()
{
    delete inp_;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	GetValue3()
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

template <typename S> 
S InputManager::GetValue3(std::map<std::string, S> map, std::string & key)
{
     typename std::map<std::string, S>::iterator S_it = map.find(key);
     if(S_it == map.end())
     {
         throw std::runtime_error("InputManager::GetValue3: " + key + " not found");
     }

     return S_it->second;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	FetchData()
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

void InputManager::FetchData()
{
    sd_map_ = inp_->GetParameters();
    sc_map_ = inp_->GetIdentifiers();
    ss_map_ = inp_->GetStrings();
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	end
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
