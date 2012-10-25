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
    LOG4CPLUS_INFO_EXT(mLogger, " Unregistering an application "<< item->getApplication()->getName());
	Items::iterator registryItemIterator = mRegistryItems.find(item->getApplication()->getName());
	mRegistryItems.erase(registryItemIterator);
}

const RegistryItem* AppMgrRegistry::getItem( const Application* app ) const
{
	return mRegistryItems.find(app->getName())->second;
}

const RegistryItem* AppMgrRegistry::getItem( const std::string& app ) const
{
	return mRegistryItems.find(app)->second;
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

const RegistryItem* AppMgrRegistry::registerApplication( const Application* app )
{
    LOG4CPLUS_INFO_EXT(mLogger, " Registering an application "<<app->getName());
	mRegistryItems.insert(Item(app->getName(), new RegistryItem(app)));
	return mRegistryItems.find(app->getName())->second;
}

};
