//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  IOFactory.h
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
#ifndef IOFactoryH
#define IOFactoryH
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

#include "MonitorManager.h"

#include "IOFactory.h"
#include "IdentifierType.h"
#include "CreateableBase.h"

#include "ConfigurationManager.h"

#include <string>
#include <map>
#include <stdexcept>

class MonitorManager;

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  template<typename Base_class> class IOFactory
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

template<typename Base_class> class IOFactory
{
	public:

		//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
		//  singleton accessor method
		//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

        static IOFactory & Instance()
		{
			static IOFactory this_factory;
			return this_factory;
		}

		//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
		//  typedefs
		//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

		typedef Base_class * (*DerivedClassCreator)();

        typedef std::pair<std::string, DerivedClassCreator> CreatePair;
		typedef std::map<std::string, DerivedClassCreator> CreateMap;

        typedef std::pair<std::string, std::string> LookUpPair;
		typedef std::map<std::string, std::string> LookUpMap;

		//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
		//  methods
		//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

		void RegisterObject
        (
        	const std::string Object_ID,
        	const std::string & Object_name, 
        	IOFactory<Base_class>::DerivedClassCreator CreateThisObject
        )
        {
        	CreateObjectsMap_[Object_name] = CreateThisObject;
        	IDLookUpMap_[Object_ID] = Object_name;
        }
		
		Base_class * CreateObject()
        {
        	ConfigurationManager & con = ConfigurationManager::Instance();
        	std::string o_type = con.GetValue<std::string>(IdentifierType(ClassName<Base_class>()));
        	
        	typename LookUpMap::const_iterator i = IDLookUpMap_.find(o_type);
        	if (i == IDLookUpMap_.end())
        	{
        		throw std::runtime_error("IOFactory:  ID " + o_type + " not found");
        	}
        
        	std::string nm = i->second;
        	DoMonitorOutput(nm);
        
        	typename CreateMap::const_iterator j = CreateObjectsMap_.find(nm);
        	if (j == CreateObjectsMap_.end())
        	{
        		throw std::runtime_error("IOFactory:  Name " + nm + " not found");
        	}
        
        	return j->second();
        }

	private:
		IOFactory()
        :   mon_(0)
        {
        	CreateObjectsMap_.clear();
        	IDLookUpMap_.clear();
        }

		IOFactory(const IOFactory &);
		IOFactory & operator=(const IOFactory &);
		
		void DoMonitorOutput(std::string & nm)
        {
        	if (mon_ == 0) mon_ = ConfigurationManager::Instance().GetMonitor();
        	mon_->OutputIOspecs(ClassName<Base_class>() + " is " + nm);
        }
		
        CreateMap CreateObjectsMap_;
        LookUpMap IDLookUpMap_;
        
        MonitorManager * mon_;
};

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
#endif
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  end of file
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXvXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

