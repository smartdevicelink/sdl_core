/*
 * AppMgrRegistry.cpp
 *
 *  Created on: Oct 4, 2012
 *      Author: vsalo
 */

#include "AppMgr/AppMgrRegistry.h"

static AppMgrRegistry& AppMgrRegistry::getInstance( )
{
	static AppMgrRegistry registry;
	return registry;
}

void AppMgrRegistry::unregisterApplication( RegistryItem& item )
{
}

AppMgrRegistry::AppMgrRegistry( )
{
	// TODO Auto-generated constructor stub

}

AppMgrRegistry::~AppMgrRegistry( )
{
	// TODO Auto-generated destructor stub
}

RegistryItem& AppMgrRegistry::registerApplication( IApplication* app )
{
}
