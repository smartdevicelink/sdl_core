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
	std::map<std::string, RegistryItem>::iterator registryItemIterator = mRegistryItems.find(item.getApplication().getName());
	mRegistryItems.erase(registryItemIterator);
}

const RegistryItem& AppMgrRegistry::getItem( const IApplication& app ) const
{
	const RegistryItem& registryItem = mRegistryItems.find(app.getName())->second;
	return registryItem;
}

const RegistryItem& AppMgrRegistry::getItem( const std::string& app ) const
{
	const RegistryItem& registryItem = mRegistryItems.find(app)->second;
	return registryItem;
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

	mRegistryItems.insert(std::pair<std::string, RegistryItem>(app.getName(), item));
	return mRegistryItems.find(app.getName())->second;
}
