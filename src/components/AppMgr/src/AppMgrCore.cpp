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
	,m_bTerminate(false)
{
}

AppMgrCore::~AppMgrCore()
{
	if(!mThreadRPCAppLinkObjectsIncoming.Join())
		mThreadRPCAppLinkObjectsIncoming.Stop();
	
	if(!mThreadRPCAppLinkObjectsOutgoing.Join())
		mThreadRPCAppLinkObjectsOutgoing.Stop();
	
	if(!mThreadRPCBusObjectsIncoming.Join())
		mThreadRPCBusObjectsIncoming.Stop();
	
	if(!mThreadRPCBusObjectsOutgoing.Join())
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
	
	while(!m_bTerminate)
	{
	}
}

void AppMgrCore::terminateThreads()
{
	m_bTerminate = true;
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
