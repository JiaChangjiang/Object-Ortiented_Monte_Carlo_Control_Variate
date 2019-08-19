//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	MonitorManager.cpp
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

#include "MonitorManager.h"
#include "MonitorBase.h"

#include "ConfigurationFactory.h"

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	structural
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
MonitorManager::MonitorManager()
:	mon_(0)
{
	mon_ = ConfigurationFactory<MonitorBase>::Instance().CreateObject();
}

MonitorManager::~MonitorManager()
{
	delete mon_;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	pimple stuff
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

void MonitorManager::OutputBanner(const std::string & strg){mon_->OutputBanner(strg);}
void MonitorManager::OutputLine(const std::string & strg){mon_->OutputLine(strg);}

void MonitorManager::OutputCounter(long j, long M){mon_->OutputCounter(j, M);}
void MonitorManager::OutputCounter(long j, long M, long interval){mon_->OutputCounter(j, M, interval);}

void MonitorManager::OutputCreateable(CreateableBase & obj) const {mon_->OutputCreateable(obj);}
void MonitorManager::OutputIOspecs(const std::string & strg) const {mon_->OutputIOspecs(strg);}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	end of file
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

