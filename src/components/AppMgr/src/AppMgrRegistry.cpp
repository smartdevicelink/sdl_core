/*
 * AppMgrRegistry.cpp
 *
 *  Created on: Oct 4, 2012
 *      Author: vsalo
 */

#include "AppMgr/AppMgrRegistry.h"

AppMgrRegistry& AppMgrRegistry::getInstance( )
{
	static AppMgrRegistry registry;
	return registry;
}

void AppMgrRegistry::unregisterApplication( RegistryItem& item )
{
	std::set<RegistryItem>::iterator registryItemIterator = mRegistryItems.find(item);
	mRegistryItems.erase(registryItemIterator);
}

AppMgrRegistry::AppMgrRegistry( )
{
	// TODO Auto-generated constructor stub

}

AppMgrRegistry::~AppMgrRegistry( )
{
	// TODO Auto-generated destructor stub
}

const RegistryItem& AppMgrRegistry::registerApplication( const IApplication& app )
{
	RegistryItem item(app);
	mRegistryItems.insert(item);
	return *mRegistryItems.find(item);
}
