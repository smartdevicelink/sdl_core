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
	:mThreadRPCAppLinkObjectsIncoming(new System::ThreadArgImpl<AppMgrCore>(*this, &AppMgrCore::handleQueueRPCAppLinkObjectsIncoming, NULL))
	,mThreadRPCAppLinkObjectsOutgoing(new System::ThreadArgImpl<AppMgrCore>(*this, &AppMgrCore::handleQueueRPCAppLinkObjectsOutgoing, NULL))
	,mThreadRPCBusObjectsIncoming(new System::ThreadArgImpl<AppMgrCore>(*this, &AppMgrCore::handleQueueRPCBusObjectsIncoming, NULL))
	,mThreadRPCBusObjectsOutgoing(new System::ThreadArgImpl<AppMgrCore>(*this, &AppMgrCore::handleQueueRPCBusObjectsOutgoing, NULL))
{
}

AppMgrCore::~AppMgrCore()
{
	mThreadRPCAppLinkObjectsIncoming.Join();
	mThreadRPCAppLinkObjectsOutgoing.Join();
	mThreadRPCBusObjectsIncoming.Join();
	mThreadRPCBusObjectsOutgoing.Join();
	
	mThreadRPCAppLinkObjectsIncoming.Stop();
	mThreadRPCAppLinkObjectsOutgoing.Stop();
	mThreadRPCBusObjectsIncoming.Stop();
	mThreadRPCBusObjectsOutgoing.Stop();
}

void AppMgrCore::onMessageReceivedCallback( MobileRPCMessage * message )
{
	mMtxRPCAppLinkObjectsIncoming.Lock();

	mQueueRPCAppLinkObjectsIncoming.push((RPCAppLinkObject *)message);
	
	mMtxRPCAppLinkObjectsIncoming.Unlock();
}

void AppMgrCore::executeThreads()
{
	mThreadRPCAppLinkObjectsIncoming.Start(false);
	mThreadRPCAppLinkObjectsOutgoing.Start(false);
	mThreadRPCBusObjectsIncoming.Start(false);
	mThreadRPCBusObjectsOutgoing.Start(false);
	
	while(true)
	{
	}
}

void* AppMgrCore::handleQueueRPCAppLinkObjectsIncoming( void* )
{
	
}

void* AppMgrCore::handleQueueRPCBusObjectsIncoming( void* )
{
}

void* AppMgrCore::handleQueueRPCAppLinkObjectsOutgoing( void* )
{
}

void* AppMgrCore::handleQueueRPCBusObjectsOutgoing( void* )
{
}

};
