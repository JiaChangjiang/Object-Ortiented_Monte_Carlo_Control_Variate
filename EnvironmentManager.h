//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  EnvironmentManager.h
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
#ifndef EnvironmentManagerH
#define EnvironmentManagerH
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

#include <stdexcept>
#include <map>
#include <string>

class EnvironmentInputterBase;

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  class EnvironmentManager
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

class EnvironmentManager
{
    public:
        explicit EnvironmentManager();
        ~EnvironmentManager();

        template <typename T, typename S> void GetValue(const T & t)
        {
            t.value = GetValue2<S>(t.name);
        }

    private:
		EnvironmentInputterBase * env_;    // owns this
		
        typedef std::map<std::string, double> SD_map;
        typedef std::map<std::string, double>::iterator SD_it;

        typedef std::map<std::string, char> SC_map;
        typedef std::map<std::string, char>::iterator SC_it;

        typedef std::map<std::string, std::string> SS_map;
        typedef std::map<std::string, std::string>::iterator SS_it;

        SD_map sd_map_;
        SC_map sc_map_;
        SS_map ss_map_;

        template <typename S> S GetValue2(std::string str)
		{
		    throw std::runtime_error("EnvironmentManager.  Map not found");
		}

        template <typename S> S GetValue3(std::map<std::string, S> map, std::string & str);

        void FetchData();
};

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  template specializations.  Declare here (namespace level),  give defn in .cpp
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

template <> char EnvironmentManager::GetValue2(std::string str);
template <> double EnvironmentManager::GetValue2(std::string str);
template <> long EnvironmentManager::GetValue2(std::string str);
template <> std::string EnvironmentManager::GetValue2(std::string str);

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
#endif
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  End
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
