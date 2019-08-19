//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  InputterConsole.h
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
#ifndef InputterConsoleH
#define InputterConsoleH
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

#include "InputterBase.h"

#include <map>
#include <string>

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  class InputterConsole
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

class InputterConsole : public InputterBase
{
    public:
        explicit InputterConsole();
        ~InputterConsole();

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

        void PromptForIDs();
        void PromptForStrings();
        void PromptForNumeric();
        
        bool GetChar(std::string prompt, char & val);
        bool GetDouble(std::string prompt, double & val);
        bool GetString(std::string prompt, std::string & val);
        
        InputterBase * inp_;
};

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
#endif
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  End
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
