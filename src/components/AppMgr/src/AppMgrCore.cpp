/*
 * AppMgr.cpp
 *
 *  Created on: Oct 4, 2012
 *      Author: vsalo
 */

#include "AppMgr/AppMgrCore.h"

AppMgrCore& AppMgrCore::getInstance( )
{
	static AppMgrCore appMgr;
	return appMgr;
}

AppMgrCore::AppMgrCore()
{
}

AppMgrCore::~AppMgrCore()
{
}
