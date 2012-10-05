/*
 * AppMgr.cpp
 *
 *  Created on: Oct 4, 2012
 *      Author: vsalo
 */

#include "AppMgr/AppMgr.h"

AppMgr& AppMgr::getInstance( )
{
	static AppMgr appMgr;
	return appMgr;
}

AppMgr::AppMgr()
{
}

AppMgr::~AppMgr()
{
}
