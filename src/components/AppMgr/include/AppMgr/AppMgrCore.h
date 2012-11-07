/*
 * AppMgr.h
 *
 *  Created on: Oct 4, 2012
 *      Author: vsalo
 */

#ifndef APPMGR_H_
#define APPMGR_H_

#include <string>
#include "AppMgr/ButtonMapping.h"
#include "AppMgr/CommandMapping.h"
#include "AppMgr/MessageMapping.h"
#include "AppMgr/ButtonCapabilities.h"

namespace AppLinkRPC{
    class ALRPCMessage;
}
class JSONHandler;
class JSONRPC2Handler;

namespace AppLinkRPC
{
    class RegisterAppInterface_request;
}

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

typedef std::pair<AppLinkRPC::ALRPCMessage*, unsigned char> Message;

class AppMgrCore
{
    friend class SubscribeButtonCmd;
public:
	virtual ~AppMgrCore();

	static AppMgrCore& getInstance();
    void pushMobileRPCMessage(AppLinkRPC::ALRPCMessage * message , unsigned char sessionID);
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
    static void handleBusRPCMessageIncoming( RPC2Communication::RPC2Command* msg, void* pThis );

    template<class Object> void handleMessage(Object message);
    const RegistryItem* registerApplication(AppLinkRPC::RegisterAppInterface_request *request , const unsigned char &sessionID);
    void unregisterApplication(const Message &msg );

	void registerApplicationOnHMI( const std::string& name );

    AppMgrCoreQueue<Message>* mQueueRPCAppLinkObjectsIncoming;
    AppMgrCoreQueue<RPC2Communication::RPC2Command*>* mQueueRPCBusObjectsIncoming;

    ButtonCapabilitiesContainer mButtonCapabilities;
    ButtonMapping    mButtonsMapping;
    CommandMapping   mCommandMapping;
    MessageMapping   mMessageMapping;
	
	JSONHandler* mJSONHandler;
    JSONRPC2Handler* mJSONRPC2Handler;
	static log4cplus::Logger mLogger;
};

} // namespace NsAppManager

#endif /* APPMGR_H_ */
