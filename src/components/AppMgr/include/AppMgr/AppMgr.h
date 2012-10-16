#ifndef APPMGR_H
#define APPMGR_H

#include "AppMgr/AppFactory.h"
#include "AppMgr/AppLinkInterface.h"
#include "AppMgr/AppMgrCore.h"
#include "AppMgr/RPCAppLinkFactory.h"
#include "AppMgr/RPCBusFactory.h"
#include "AppMgr/AppMgrRegistry.h"
#include "JSONHandler/IRPCMessagesObserver.h"
#include "mb_controller.hpp"

namespace NsAppManager
{

class AppMgr: public IRPCMessagesObserver, public NsMessageBroker::CMessageBrokerController
{
public:

	/**
	 * \brief needs to be called before getInstance.
	 * \param address address to bind to.
	 * \param port port to bind to.
	 * \param name name to bind to.
	 */
	static void setParams(const std::string& address, uint16_t port, std::string name);
	
	static AppMgr& getInstance();
	
	virtual void onMessageReceivedCallback( MobileRPCMessage * message );

	/**
	 * \brief pure virtual method to process response.
	 * \param method method name which has been called.
	 * \param root JSON message.
	 */
	virtual void processResponse(std::string method, Json::Value& root);
	
	/**
	 * \brief pure virtual method to process request.
	 * \param root JSON message.
	 */
	virtual void processRequest(Json::Value& root);
	
	/**
	 * \brief Process notification message.
	 * \brief Notify subscribers about property change.
	 * expected notification format example:
	 * \code
	 * {"jsonrpc": "2.0", "method": "<ComponentName>.<NotificationName>", "params": <list of params>}
	 * \endcode
	 * \param root JSON message.
	 */
	virtual void processNotification(Json::Value& root);
	
private:
	virtual ~AppMgr();
	AppMgr(const std::string& address, uint16_t port, std::string name);

	AppLinkInterface& mAppLinkInterface;
	AppMgrRegistry& mAppMgrRegistry;
	AppMgrCore& mAppMgrCore;
	RPCAppLinkFactory& mRPCAppLinkFactory;
	RPCBusFactory& mRPCBusFactory;
	AppFactory& mAppFactory;

	static std::string mAddress;
	static uint16_t mPort;
	static std::string mName;
	static bool m_bInitialized;
};

}; // namespace NsAppManager

#endif // APPMGR_H
