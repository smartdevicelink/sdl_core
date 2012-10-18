/*
 * AppMgr.h
 *
 *  Created on: Oct 4, 2012
 *      Author: vsalo
 */

#ifndef APPMGR_H_
#define APPMGR_H_

#include "system.h"
#include <queue>

class RegisterAppInterface;
class MobileRPCMessage;
class JSONHandler;

namespace NsAppManager
{

class RPCBusObject;
	
class AppMgrCore
{
public:
	virtual ~AppMgrCore();

	static AppMgrCore& getInstance();
	void pushMobileRPCMessage( MobileRPCMessage * message );

	void executeThreads();
	void terminateThreads();

	void setJsonHandler(JSONHandler* handler);
	JSONHandler* getJsonHandler( ) const;

private:
	AppMgrCore();

	void handleMobileRPCMessage( MobileRPCMessage* msg );
	void handleBusRPCMessage( RPCBusObject* msg );
	void registerApplication( RegisterAppInterface* msg );
	void sendMobileRPCResponse( MobileRPCMessage* msg );
	void enqueueOutgoingMobileRPCMessage( MobileRPCMessage * message );

	void* handleQueueRPCAppLinkObjectsIncoming( void* );
	void* handleQueueRPCBusObjectsIncoming( void* );
	void* handleQueueRPCAppLinkObjectsOutgoing( void* );
	void* handleQueueRPCBusObjectsOutgoing( void* );
	
	std::queue< MobileRPCMessage* > mQueueRPCAppLinkObjectsIncoming;
	std::queue< MobileRPCMessage* > mQueueRPCAppLinkObjectsOutgoing;
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
	JSONHandler* mJSONHandler;
};

}; // namespace NsAppManager

#endif /* APPMGR_H_ */
