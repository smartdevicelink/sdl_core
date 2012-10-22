/*
 * RegistryItem.cpp
 *
 *  Created on: Oct 4, 2012
 *      Author: vsalo
 */

#include "AppMgr/RegistryItem.h"

namespace NsAppManager
{
	
RegistryItem::RegistryItem( const Application* app )
: mApplication(app)
{
}

RegistryItem::RegistryItem( const RegistryItem& item )
: mApplication(item.getApplication())
{
}

RegistryItem::~RegistryItem( )
{
	if(mApplication)
	{
		delete mApplication;
		mApplication = 0;
	}

	for(Policies::iterator it = mAppPolicies.begin(); it != mAppPolicies.end(); it++)
	{
		if( *it )
		{
			delete *it;
		}
	}
	
	mAppPolicies.clear();
}

const AppPolicy* RegistryItem::registerPolicy( const std::string& hash )
{
	AppPolicy* policy = new AppPolicy(hash);
	mAppPolicies.insert(policy);
	return *mAppPolicies.find(policy);
}

void RegistryItem::unregisterPolicy( AppPolicy* policy )
{
	Policies::iterator policyIterator = mAppPolicies.find(policy);
	mAppPolicies.erase(policyIterator);
}

const Application* RegistryItem::getApplication( ) const
{
	return mApplication;
}

bool RegistryItem::operator <(const RegistryItem& item2 ) const
{
	return this->getApplication()->getName() < item2.getApplication()->getName();
}

RegistryItem::Policies RegistryItem::getApplicationPolicies( const Application* app ) const
{
	Policies policySet;
	return policySet;
}

RegistryItem::Policies RegistryItem::getApplicationPolicies(
		const std::string& app ) const
{
	Policies policySet;
	return policySet;
}

};
