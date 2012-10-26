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
#include <map>
#include "JSONHandler/GetCapabilitiesResponse.h"

class RegisterAppInterface_request;
class RegisterAppInterface_response;
class SubscribeButton_request;
class ALRPCMessage;
class JSONHandler;
class JSONRPC2Handler;
class UnsubscribeButton_request;

namespace RPC2Communication
{
	class RPC2Command;
}

namespace log4cplus
{
	class Logger;
}

namespace NsAppManager
{

class RegistryItem;

struct Comparer {
    bool operator() (const ButtonName &b1, const ButtonName &b2) const;
};

typedef std::vector<ButtonCapabilities> Capabilities;
typedef std::map<ButtonName, RegistryItem*, Comparer> ButtonMap;
typedef std::pair<ButtonName, RegistryItem*> ButtonMapItem;
typedef std::pair<ALRPCMessage*, unsigned char> Message;
typedef std::map<int, unsigned char> MessagesToSessions;
typedef std::pair<int, unsigned char> MessageToSession;

class AppMgrCore
{
public:
	virtual ~AppMgrCore();

	static AppMgrCore& getInstance();
    void pushMobileRPCMessage(ALRPCMessage * message , unsigned char sessionID);
	void pushRPC2CommunicationMessage( RPC2Communication::RPC2Command * message );

	void executeThreads();
	void terminateThreads();

	void setJsonHandler(JSONHandler* handler);
	JSONHandler* getJsonHandler( ) const;

    void setJsonRPC2Handler(JSONRPC2Handler* handler);
    JSONRPC2Handler* getJsonRPC2Handler( ) const;

private:
	AppMgrCore();
    AppMgrCore(const AppMgrCore&);

    void handleMobileRPCMessage(const Message &message );
	void handleBusRPCMessageIncoming( RPC2Communication::RPC2Command* msg );
    void handleBusRPCMessageOutgoing( RPC2Communication::RPC2Command* msg );
    const ALRPCMessage* queryInfoForRegistration( const RegistryItem* registryItem );
    const RegistryItem* registerApplication(const Message &msg );
    void unregisterApplication(const Message &msg );
    void subscribeButton(const Message &msg );
    void unsubscribeButton(const Message &msg );
    unsigned char findSessionIdSubscribedToButton(ButtonName appName) const;
    unsigned char findSessionIdByMessage(int messageId) const;
    void clearButtonSubscribtion(unsigned char sessionID);
    void sendMobileRPCResponse( const Message &msg );
    void sendHMIRPC2Response( RPC2Communication::RPC2Command * msg );
    void enqueueOutgoingMobileRPCMessage( const Message &message );
	void enqueueOutgoingBusRPCMessage( RPC2Communication::RPC2Command * message );

    void mapMessageToSession( int messageId, unsigned char sessionId );

	void registerApplicationOnHMI( const std::string& name );

	void setButtonCapabilities( RPC2Communication::GetCapabilitiesResponse* msg );
    const Capabilities& getButtonCapabilities() const;

	void* handleQueueRPCAppLinkObjectsIncoming( void* );
	void* handleQueueRPCBusObjectsIncoming( void* );
	void* handleQueueRPCAppLinkObjectsOutgoing( void* );
	void* handleQueueRPCBusObjectsOutgoing( void* );
	
    std::queue< Message > mQueueRPCAppLinkObjectsIncoming;
    std::queue< Message > mQueueRPCAppLinkObjectsOutgoing;
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
    ButtonMap    mButtonsMapping;
    MessagesToSessions mMessagesToSessionsMap;
	
	bool m_bTerminate;
	JSONHandler* mJSONHandler;
    JSONRPC2Handler* mJSONRPC2Handler;
	static log4cplus::Logger mLogger;
};

}; // namespace NsAppManager

#endif /* APPMGR_H_ */
