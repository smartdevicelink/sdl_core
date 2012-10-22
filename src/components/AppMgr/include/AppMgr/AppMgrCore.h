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
#include <string>

class RegisterAppInterface;
class MobileRPCMessage;
class JSONHandler;

namespace log4cplus
{
	class Logger;
};

namespace NsAppManager
{

class RPCBusObject;
class RegistryItem;
	
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
	const RegistryItem* registerApplication( RegisterAppInterface* msg );
	void sendMobileRPCResponse( MobileRPCMessage* msg );
	void enqueueOutgoingMobileRPCMessage( MobileRPCMessage * message );
	MobileRPCMessage* queryInfoForRegistration( const RegistryItem* registryItem );
	void enqueueOutgoingBusRPCMessage( RPCBusObject * message );

	void registerApplicationOnHMI( const std::string& name );

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
	const log4cplus::Logger& mLogger;
};

}; // namespace NsAppManager

#endif /* APPMGR_H_ */
