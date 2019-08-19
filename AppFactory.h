//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  AppFactory.h
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
#ifndef AppFactoryH
#define AppFactoryH
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

#include "IO_Manager.h"
#include "CreateableBase.h"
#include "IdentifierType.h"
#include "ConfigurationManager.h"
#include "MonitorManager.h"

#include <string>
#include <map>
#include <stdexcept>

class MonitorManager;

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  template<typename Base_class> class AppFactory
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

template<typename Base_class> class AppFactory
{
	public:

		//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
		//  singleton accessor method
		//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

        static AppFactory & Instance()
		{
			static AppFactory this_factory;
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
        	const std::string & Object_ID,
        	const std::string & Object_name, 
        	AppFactory<Base_class>::DerivedClassCreator CreateThisObject
        )
        {
        	CreateObjectsMap_[Object_name] = CreateThisObject;
        	IDLookUpMap_[Object_ID] = Object_name;
        }

		Base_class * CreateObject()
        {
        	IO_Manager & io = IO_Manager::Instance();
        	std::string o_type = io.GetValue<std::string>(IdentifierType(ClassName<Base_class>()));
        
        	typename LookUpMap::const_iterator i = IDLookUpMap_.find(o_type);
        	if (i == IDLookUpMap_.end())
        	{
        		throw std::runtime_error("AppFactory:  ID " + o_type + " not found");
        	}
        
        	std::string nm = i->second;
        
        	typename CreateMap::const_iterator j = CreateObjectsMap_.find(nm);
        	if (j == CreateObjectsMap_.end())
        	{
        		throw std::runtime_error("AppFactory:  Name " + nm + " not found");
        	}
        
        	Base_class * CreatedObj = j->second();
        	DoMonitorOutput(*CreatedObj);
        	return CreatedObj;
        }

	private:
		AppFactory()
        :   mon_(0)
        {
        	CreateObjectsMap_.clear();
        	IDLookUpMap_.clear();
        }

		AppFactory(const AppFactory &);
		//AppFactory & operator=(const AppFactory &);

        CreateMap CreateObjectsMap_;
        LookUpMap IDLookUpMap_;
        
        MonitorManager * mon_;

		void DoMonitorOutput(Base_class & obj)
        {
        	if (mon_ == 0) mon_ = ConfigurationManager::Instance().GetMonitor();  // does not own
        	mon_->OutputCreateable(obj);
        }
};

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
#endif
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  end of file
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

