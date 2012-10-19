/*
 * AppMgrRegistry.cpp
 *
 *  Created on: Oct 4, 2012
 *      Author: vsalo
 */

#include "AppMgr/AppMgrRegistry.h"

namespace NsAppManager
{
	
AppMgrRegistry& AppMgrRegistry::getInstance( )
{
	static AppMgrRegistry registry;
	return registry;
}

void AppMgrRegistry::unregisterApplication( RegistryItem* item )
{
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
}

AppMgrRegistry::~AppMgrRegistry( )
{
	for(Items::iterator it = mRegistryItems.begin(); it != mRegistryItems.end(); it++)
	{
		if( it->second )
		{
			delete it->second;
			it->second = 0;
		}
	}

	mRegistryItems.clear();
}

const RegistryItem* AppMgrRegistry::registerApplication( const Application* app )
{
	mRegistryItems.insert(Item(app->getName(), new RegistryItem(app)));
	return mRegistryItems.find(app->getName())->second;
}

};
