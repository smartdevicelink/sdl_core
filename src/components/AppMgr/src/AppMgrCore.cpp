/*
 * AppMgr.cpp
 *
 *  Created on: Oct 4, 2012
 *      Author: vsalo
 */

#include "AppMgr/AppMgrCore.h"

namespace NsAppManager
{
	
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

void AppMgrCore::onMessageReceivedCallback( MobileRPCMessage * message )
{
}

void AppMgrCore::handleQueueRPCAppLinkObjectsIncoming( void* )
{
}

void AppMgrCore::handleQueueRPCBusObjectsIncoming( void* )
{
}

void AppMgrCore::handleQueueRPCAppLinkObjectsOutgoing( void* )
{
}

void AppMgrCore::handleQueueRPCBusObjectsOutgoing( void* )
{
}

};
