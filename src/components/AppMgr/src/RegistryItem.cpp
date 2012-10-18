/*
 * RegistryItem.cpp
 *
 *  Created on: Oct 4, 2012
 *      Author: vsalo
 */

#include "AppMgr/RegistryItem.h"

namespace NsAppManager
{
	
RegistryItem::RegistryItem( const Application& app )
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

const Application& RegistryItem::getApplication( ) const
{
	return mApplication;
}

bool RegistryItem::operator <(const RegistryItem& item2 ) const
{
	return this->getApplication().getName() < item2.getApplication().getName();
}

std::set< AppPolicy > RegistryItem::getApplicationPolicies(	const Application& app ) const
{
	std::set< AppPolicy > policySet;
	return policySet;
}

std::set< AppPolicy > RegistryItem::getApplicationPolicies(
		const std::string& app ) const
{
	std::set< AppPolicy > policySet;
	return policySet;
}

};
