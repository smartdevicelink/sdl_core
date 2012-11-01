/*
 * AppMgr.h
 *
 *  Created on: Oct 4, 2012
 *      Author: vsalo
 */

#ifndef APPMGR_H_
#define APPMGR_H_

#include <string>
#include <map>
#include "AppMgr/ButtonMapping.h"
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
template< class QueueType >
class AppMgrCoreQueue;

typedef std::vector<ButtonCapabilities> Capabilities;
typedef std::pair<ALRPCMessage*, unsigned char> Message;
typedef std::map<int, unsigned char> MessagesToSessions;
typedef std::pair<int, unsigned char> MessageToSession;

class AppMgrCore
{
    friend class SubscribeButtonCmd;
public:
	virtual ~AppMgrCore();

	static AppMgrCore& getInstance();
    void pushMobileRPCMessage(ALRPCMessage * message , unsigned char sessionID);
	void pushRPC2CommunicationMessage( RPC2Communication::RPC2Command * message );

	void executeThreads();

	void setJsonHandler(JSONHandler* handler);
	JSONHandler* getJsonHandler( ) const;

    void setJsonRPC2Handler(JSONRPC2Handler* handler);
    JSONRPC2Handler* getJsonRPC2Handler( ) const;

private:
	AppMgrCore();
    AppMgrCore(const AppMgrCore&);

    static void handleMobileRPCMessage(Message message, void* pThis);
    static void handleMobileRPCNotification(ALRPCMessage* message, void* pThis );
    static void handleBusRPCMessageIncoming( RPC2Communication::RPC2Command* msg, void* pThis );
    static void handleBusRPCMessageOutgoing( RPC2Communication::RPC2Command* msg, void* pThis );
    template<class Object> void handleMessage(Object message);
    const ALRPCMessage* queryInfoForRegistration( const RegistryItem* registryItem );
    const RegistryItem* registerApplication(const Message &msg );
    void unregisterApplication(const Message &msg );
    unsigned char findSessionIdByMessage(int messageId) const;
    void mapMessageToSession( int messageId, unsigned char sessionId );
    void removeMessageToSessionMapping( int messageId );

	void registerApplicationOnHMI( const std::string& name );

	void setButtonCapabilities( RPC2Communication::GetCapabilitiesResponse* msg );
    const Capabilities& getButtonCapabilities() const;

    AppMgrCoreQueue<Message>* mQueueRPCAppLinkObjectsIncoming;
    AppMgrCoreQueue<RPC2Communication::RPC2Command*>* mQueueRPCBusObjectsIncoming;

	Capabilities mButtonCapabilities;
    ButtonMapping    mButtonsMapping;
    MessagesToSessions mMessagesToSessionsMap;
	
	JSONHandler* mJSONHandler;
    JSONRPC2Handler* mJSONRPC2Handler;
	static log4cplus::Logger mLogger;
};

} // namespace NsAppManager

#endif /* APPMGR_H_ */
