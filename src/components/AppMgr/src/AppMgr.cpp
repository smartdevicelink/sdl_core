/*
 * AppMgr.cpp
 *
 *  Created on: Oct 4, 2012
 *      Author: vsalo
 */

#include "AppMgr.h"

static AppMgr& AppMgr::getInstance( )
{
	AppMgr appMgr;
	return appMgr;
}

AppMgr::AppMgr()
{
}

AppMgr::~AppMgr()
{
}
