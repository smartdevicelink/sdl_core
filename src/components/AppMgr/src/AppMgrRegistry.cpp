/*
 * AppMgrRegistry.cpp
 *
 *  Created on: Oct 4, 2012
 *      Author: vsalo
 */

#include "AppMgr/AppMgrRegistry.h"
#include "LoggerHelper.hpp"

namespace NsAppManager
{

log4cplus::Logger AppMgrRegistry::mLogger = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("AppMgrRegistry"));
	
AppMgrRegistry& AppMgrRegistry::getInstance( )
{
	static AppMgrRegistry registry;
	return registry;
}

void AppMgrRegistry::unregisterApplication( RegistryItem* item )
{
    if(!item)
    {
        LOG4CPLUS_ERROR_EXT(mLogger, " Trying to unregister null app!");
        return;
    }
    LOG4CPLUS_INFO_EXT(mLogger, " Unregistering an application "<< item->getApplication()->getName());
    Items::const_iterator registryItemIterator = mRegistryItems.find(item->getApplication()->getName());
	mRegistryItems.erase(registryItemIterator);
}

RegistryItem *AppMgrRegistry::getItem( const Application* app ) const
{
    if(!app)
    {
        LOG4CPLUS_ERROR_EXT(mLogger, " Getting registry item for null application!");
        return 0;
    }
    return getItem( app->getName() );
}

RegistryItem *AppMgrRegistry::getItem( const std::string& app ) const
{
    Items::const_iterator it = mRegistryItems.find(app);
    if(it != mRegistryItems.end())
    {
        return it->second;
    }
    LOG4CPLUS_ERROR_EXT(mLogger, " Cannot find registry item by the name "<< app);
    return 0;
}

RegistryItem *AppMgrRegistry::getItem(unsigned char sessionID) const
{
    for(Items::const_iterator it = mRegistryItems.begin(); it != mRegistryItems.end(); it++)
    {
        RegistryItem* item =  it->second;
        if(!item)
        {
            LOG4CPLUS_ERROR_EXT(mLogger, " NULL-application registered for this session ID - "<<sessionID);
            return 0;
        }
        const Application* app = item->getApplication();
        if(app->getSessionID() == sessionID)
        {
            return item;
        }
    }
    LOG4CPLUS_ERROR_EXT(mLogger, " No application found with the session id "<<sessionID);
    return 0;
}

AppMgrRegistry::AppMgrRegistry( )
{
    LOG4CPLUS_INFO_EXT(mLogger, " Created a registry!");
}

AppMgrRegistry::AppMgrRegistry(const AppMgrRegistry &)
{
}

AppMgrRegistry::~AppMgrRegistry( )
{
    LOG4CPLUS_INFO_EXT(mLogger, " Destructing a registry!");
	for(Items::iterator it = mRegistryItems.begin(); it != mRegistryItems.end(); it++)
	{
		if( it->second )
		{
			delete it->second;
			it->second = 0;
		}
	}

	mRegistryItems.clear();
    LOG4CPLUS_INFO_EXT(mLogger, " Destructed a registry!");
}

const RegistryItem* AppMgrRegistry::registerApplication( Application* app )
{
    if(!app)
    {
        LOG4CPLUS_ERROR_EXT(mLogger, " Trying to register a null-application!");
        return 0;
    }
    LOG4CPLUS_INFO_EXT(mLogger, " Registering an application "<<app->getName());
	mRegistryItems.insert(Item(app->getName(), new RegistryItem(app)));
	return mRegistryItems.find(app->getName())->second;
}

}
