/*
 * AppPolicy.cpp
 *
 *  Created on: Oct 5, 2012
 *      Author: vsalo
 */

#include "AppMgr/AppPolicy.h"
#include "LoggerHelper.hpp"

namespace NsAppManager
{
log4cplus::Logger AppPolicy::mLogger = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("AppPolicy"));
	
AppPolicy::AppPolicy( const std::string& policy )
: mPolicy(policy)
{
    LOG4CPLUS_INFO_EXT(mLogger, " Created an app policy "<<policy);
}

AppPolicy::~AppPolicy( )
{
    LOG4CPLUS_INFO_EXT(mLogger, " Destroyed an app policy "<<mPolicy);
}

bool AppPolicy::operator <( const AppPolicy& item2 ) const
{
	return this->getPolicyHash() < item2.getPolicyHash();
}

const std::string& AppPolicy::getPolicyHash( ) const
{
    return mPolicy;
}

AppPolicy::AppPolicy(const AppPolicy &policy)
    : mPolicy(policy.getPolicyHash())
{
}

};
