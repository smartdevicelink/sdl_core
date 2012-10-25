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
#include "JSONHandler/GetCapabilitiesResponse.h"

class RegisterAppInterface_request;
class RegisterAppInterface_response;
class SubscribeButton_request;
class ALRPCMessage;
class JSONHandler;
class UnsubscribeButton_request;

namespace RPC2Communication
{
	class RPC2Command;
};

namespace log4cplus
{
	class Logger;
};

namespace NsAppManager
{

class RegistryItem;

typedef std::vector<ButtonCapabilities> Capabilities;

class AppMgrCore
{
public:
	virtual ~AppMgrCore();

	static AppMgrCore& getInstance();
	void pushMobileRPCMessage( ALRPCMessage * message );
	void pushRPC2CommunicationMessage( RPC2Communication::RPC2Command * message );

	void executeThreads();
	void terminateThreads();

	void setJsonHandler(JSONHandler* handler);
	JSONHandler* getJsonHandler( ) const;

private:
	AppMgrCore();
    AppMgrCore(const AppMgrCore&);

	void handleMobileRPCMessage( ALRPCMessage* msg );
	void handleBusRPCMessageIncoming( RPC2Communication::RPC2Command* msg );
	const RegistryItem* registerApplication( RegisterAppInterface_request* msg );
    void subscribeButton(SubscribeButton_request *msg );
    void unsubscribeButton( UnsubscribeButton_request* msg );
	void sendMobileRPCResponse( ALRPCMessage* msg );
	void enqueueOutgoingMobileRPCMessage( ALRPCMessage * message );
    const ALRPCMessage* queryInfoForRegistration( const RegistryItem* registryItem );
	void enqueueOutgoingBusRPCMessage( RPC2Communication::RPC2Command * message );

	void registerApplicationOnHMI( const std::string& name );

	void setButtonCapabilities( RPC2Communication::GetCapabilitiesResponse* msg );
    const Capabilities& getButtonCapabilities() const;

	void* handleQueueRPCAppLinkObjectsIncoming( void* );
	void* handleQueueRPCBusObjectsIncoming( void* );
	void* handleQueueRPCAppLinkObjectsOutgoing( void* );
	void* handleQueueRPCBusObjectsOutgoing( void* );
	
	std::queue< ALRPCMessage* > mQueueRPCAppLinkObjectsIncoming;
	std::queue< ALRPCMessage* > mQueueRPCAppLinkObjectsOutgoing;
	std::queue< RPC2Communication::RPC2Command* > mQueueRPCBusObjectsIncoming;
	std::queue< RPC2Communication::RPC2Command* > mQueueRPCBusObjectsOutgoing;

	System::Mutex mMtxRPCAppLinkObjectsIncoming;
	System::Mutex mMtxRPCAppLinkObjectsOutgoing;
	System::Mutex mMtxRPCBusObjectsIncoming;
	System::Mutex mMtxRPCBusObjectsOutgoing;

	System::Thread mThreadRPCAppLinkObjectsIncoming;
	System::Thread mThreadRPCAppLinkObjectsOutgoing;
	System::Thread mThreadRPCBusObjectsIncoming;
	System::Thread mThreadRPCBusObjectsOutgoing;

	Capabilities mButtonCapabilities;
	
	bool m_bTerminate;
	JSONHandler* mJSONHandler;
	static log4cplus::Logger mLogger;
};

}; // namespace NsAppManager

#endif /* APPMGR_H_ */
