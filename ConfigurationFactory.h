//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  ConfigurationFactory.h
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
#ifndef ConfigurationFactoryH
#define ConfigurationFactoryH
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

#include "SetUpManager.h"
#include "CreateableBase.h"

#include "IdentifierType.h"

#include <string>
#include <map>
#include <stdexcept>

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  template<typename Base_class> class ConfigurationFactory
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

template<typename Base_class> class ConfigurationFactory
{
	public:

		//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
		//  singleton accessor method
		//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

        static ConfigurationFactory & Instance()
		{
 			static ConfigurationFactory this_factory;
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
        	ConfigurationFactory<Base_class>::DerivedClassCreator CreateThisObject
        )
        {
        	CreatePair this_pair(Object_name, CreateThisObject);
        	CreateObjectsMap_.insert(this_pair);
        	
        	LookUpPair that_pair(Object_ID, Object_name);
        	IDLookUpMap_.insert(that_pair);
        }

		friend class EnvironmentManager;
		friend class MonitorManager;

	private:
		ConfigurationFactory()
        {
        	CreateObjectsMap_.clear();
        	IDLookUpMap_.clear();
        }
		
		ConfigurationFactory(const ConfigurationFactory &);
		ConfigurationFactory & operator=(const ConfigurationFactory &);
		
		Base_class * CreateObject()  //Accessable to EnvironmentManager & MonitorManager
        {
        	SetUpManager & set = SetUpManager::Instance();
        	std::string o_type = set.GetValue<std::string>(IdentifierType(ClassName<Base_class>()));
        
        	typename LookUpMap::const_iterator i = IDLookUpMap_.find(o_type);
        	if (i == IDLookUpMap_.end())
        	{
        		throw std::runtime_error("ConfigurationFactory:  ID " + o_type + " not found");
        	}
        
        	std::string nm = i->second;
        	typename CreateMap::const_iterator j = CreateObjectsMap_.find(nm);
        	if (j == CreateObjectsMap_.end())
        	{
        		throw std::runtime_error("ConfigurationFactory:  Name " + nm + " not found");
        	}
        
        	return j->second();
        }
        
        CreateMap CreateObjectsMap_;
        LookUpMap IDLookUpMap_;
};

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  Must (i)  #include the .cpp in the .h
//       (ii) not add the .cpp to the project
//  for template classes
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
#endif
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  end of file
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

