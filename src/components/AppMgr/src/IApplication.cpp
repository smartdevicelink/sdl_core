/*
 * IApplication.cpp
 *
 *  Created on: Oct 5, 2012
 *      Author: vsalo
 */

#include "AppMgr/IApplication.h"

namespace NsAppManager
{
	
IApplication::IApplication( const std::string& name )
: mName(name)
, mPriority(NONE)
{
}

IApplication::IApplication( const IApplication& app )
: mPriority(NONE)
{
}

const std::string& IApplication::getName( ) const
{
	return mName;
}

};
