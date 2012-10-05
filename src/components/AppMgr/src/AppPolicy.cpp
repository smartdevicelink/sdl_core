/*
 * AppPolicy.cpp
 *
 *  Created on: Oct 5, 2012
 *      Author: vsalo
 */

#include "AppMgr/AppPolicy.h"

AppPolicy::AppPolicy( const std::string& policy )
: mPolicy(policy)
{

}

AppPolicy::~AppPolicy( )
{
	// TODO Auto-generated destructor stub
}

bool AppPolicy::operator <( const AppPolicy& item2 ) const
{
	return this->getPolicyHash() < item2.getPolicyHash();
}

const std::string& AppPolicy::getPolicyHash( ) const
{
	return mPolicy;
}
