//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  InputterCoded.h
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
#ifndef InputterCodedH
#define InputterCodedH
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

#include "InputterBase.h"

#include <map>
#include <string>

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  class InputterCoded
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

class InputterCoded : public InputterBase
{
    public:
        explicit InputterCoded();

        std::map<std::string, double> GetParameters() const {return sd_map_;}
        std::map<std::string, char> GetIdentifiers() const {return sc_map_;}
        std::map<std::string, std::string> GetStrings() const {return ss_map_;}

    private:
        typedef std::map<std::string, double> SD_map;
        typedef std::map<std::string, double>::iterator SD_it;
        
        typedef std::map<std::string, char> SC_map;
        typedef std::map<std::string, char>::iterator SC_it;

        typedef std::map<std::string, std::string> SS_map;
        typedef std::map<std::string, std::string>::iterator SS_it;

        SD_map sd_map_;
        SC_map sc_map_;
        SS_map ss_map_;
};

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
#endif
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  End
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
