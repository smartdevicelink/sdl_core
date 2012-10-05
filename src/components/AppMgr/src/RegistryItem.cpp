/*
 * RegistryItem.cpp
 *
 *  Created on: Oct 4, 2012
 *      Author: vsalo
 */

#include "AppMgr/RegistryItem.h"

RegistryItem::RegistryItem( const IApplication& app )
: mApplication(app)
{
}

RegistryItem::~RegistryItem( )
{
	// TODO Auto-generated destructor stub
}

const AppPolicy& RegistryItem::registerPolicy( const std::string& hash )
{
	AppPolicy policy(hash);
	mAppPolicies.insert(policy);
	return *mAppPolicies.find(policy);
}

void RegistryItem::unregisterPolicy( AppPolicy& policy )
{
	std::set<AppPolicy>::iterator policyIterator = mAppPolicies.find(policy);
	mAppPolicies.erase(policyIterator);
}

const IApplication& RegistryItem::getApplication( ) const
{
	return mApplication;
}

bool RegistryItem::operator <(const RegistryItem& item2 ) const
{
	return this->getApplication().getName() < item2.getApplication().getName();
}
