/*
 * RegistryItem.cpp
 *
 *  Created on: Oct 4, 2012
 *      Author: vsalo
 */

#include "AppMgr/RegistryItem.h"
#include "LoggerHelper.hpp"

namespace NsAppManager
{

log4cplus::Logger RegistryItem::mLogger = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("RegistryItem"));
	
RegistryItem::RegistryItem( Application* app )
: mApplication(app)
{
    if(!app)
    {
        LOG4CPLUS_ERROR_EXT(mLogger, "No application to register! Please specify a valid application instance!");
        return;
    }
    LOG4CPLUS_INFO_EXT(mLogger, " RegistryItem constructed for the application "<<app?app->getName():"NULL!");
}

RegistryItem::RegistryItem( const RegistryItem& item )
: mApplication(item.getApplication())
{
}

RegistryItem::~RegistryItem( )
{
    LOG4CPLUS_INFO_EXT(mLogger, " RegistryItem destroyed for the application "<<mApplication?mApplication->getName():"NULL!");
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
    LOG4CPLUS_INFO_EXT(mLogger, " Registering a policy "<<hash);
	AppPolicy* policy = new AppPolicy(hash);
	mAppPolicies.insert(policy);
	return *mAppPolicies.find(policy);
}

void RegistryItem::unregisterPolicy( AppPolicy* policy )
{
    if(!policy)
    {
        LOG4CPLUS_ERROR_EXT(mLogger, " Trying to unregister null policy!");
        return;
    }
    LOG4CPLUS_INFO_EXT(mLogger, " Unregistering a policy "<<policy->getPolicyHash());
	Policies::iterator policyIterator = mAppPolicies.find(policy);
	mAppPolicies.erase(policyIterator);
}

Application* RegistryItem::getApplication( ) const
{
    LOG4CPLUS_ERROR_EXT(mLogger, "About to return a null application: a null ptr exception may occur right after this line!");
	return mApplication;
}

bool RegistryItem::operator <(const RegistryItem& item2 ) const
{
	return this->getApplication()->getName() < item2.getApplication()->getName();
}

RegistryItem::Policies RegistryItem::getApplicationPolicies( const Application* app ) const
{
    if(!app)
    {
        LOG4CPLUS_ERROR_EXT(mLogger, " Trying to get policies of null application!");
     //   return;
    }
	Policies policySet;
	return policySet;
}

RegistryItem::Policies RegistryItem::getApplicationPolicies(
		const std::string& app ) const
{
	Policies policySet;
	return policySet;
}

}
