/*
 * AppMgrRegistry.h
 *
 *  Created on: Oct 4, 2012
 *      Author: vsalo
 */

#ifndef APPMGRREGISTRY_H_
#define APPMGRREGISTRY_H_

#include "IApplication.h"
#include "RegistryItem.h"

#include <set>

class AppMgrRegistry
{
public:
	AppMgrRegistry( );
	~AppMgrRegistry( );

	RegistryItem& registerApplication( IApplication* app );
	void unregisterApplication( RegistryItem& item  );

private:
	std::set<RegistryItem> mRegistryItems;
};

#endif /* APPMGRREGISTRY_H_ */
