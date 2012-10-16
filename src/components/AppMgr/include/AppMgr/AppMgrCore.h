/*
 * AppMgr.h
 *
 *  Created on: Oct 4, 2012
 *      Author: vsalo
 */

#ifndef APPMGR_H_
#define APPMGR_H_

#include "IApplication.h"
#include "AppMgrRegistry.h"
#include "AppPolicy.h"
#include "RPCAppLinkObject.h"
#include "RPCBusObject.h"
#include "JSONHandler/MobileRPCMessage.h"
#include "system.h"

#include <queue>

namespace NsAppManager
{
	
class AppMgrCore
{
public:
	virtual ~AppMgrCore();

	static AppMgrCore& getInstance();
	void pushMobileRPCMessage( MobileRPCMessage * message );

	void executeThreads();
	void terminateThreads();

private:
	AppMgrCore();

	void handleMessage( MobileRPCMessage* msg );
	void registerApplication( MobileRPCMessage* msg );

	void* handleQueueRPCAppLinkObjectsIncoming( void* );
	void* handleQueueRPCBusObjectsIncoming( void* );
	void* handleQueueRPCAppLinkObjectsOutgoing( void* );
	void* handleQueueRPCBusObjectsOutgoing( void* );
	
	std::queue< RPCAppLinkObject* > mQueueRPCAppLinkObjectsIncoming;
	std::queue< RPCAppLinkObject* > mQueueRPCAppLinkObjectsOutgoing;
	std::queue< RPCBusObject* > mQueueRPCBusObjectsIncoming;
	std::queue< RPCBusObject* > mQueueRPCBusObjectsOutgoing;

	System::Mutex mMtxRPCAppLinkObjectsIncoming;
	System::Mutex mMtxRPCAppLinkObjectsOutgoing;
	System::Mutex mMtxRPCBusObjectsIncoming;
	System::Mutex mMtxRPCBusObjectsOutgoing;

	System::Thread mThreadRPCAppLinkObjectsIncoming;
	System::Thread mThreadRPCAppLinkObjectsOutgoing;
	System::Thread mThreadRPCBusObjectsIncoming;
	System::Thread mThreadRPCBusObjectsOutgoing;

	bool m_bTerminate;
};

}; // namespace NsAppManager

#endif /* APPMGR_H_ */
