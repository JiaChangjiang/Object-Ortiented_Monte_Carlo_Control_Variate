//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  InputterFile.h
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
#ifndef InputterFileH
#define InputterFileH
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

#include "InputterBase.h"

#include <iostream>
#include <fstream>
#include <map>
#include <string>

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  class InputterFile.
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

class InputterFile : public InputterBase
{
    public:
        InputterFile();
        ~InputterFile();
        
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

        std::string input_file_path_;

        std::fstream i_stream_;    //Stream for input

        bool Data_Fetched_;

		void FetchData();

		template<typename T> std::pair<std::string, T> FetchPair()
		{
		    std::pair<std::string, T> this_pair;
			i_stream_ >> this_pair.first;
			i_stream_ >> this_pair.second;
			return this_pair;
		}

 		void WriteMaps();   // utilities
        void WriteSDmap();
        void WriteSCmap();
        void WriteSSmap();
};

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
#endif
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  end of file
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

