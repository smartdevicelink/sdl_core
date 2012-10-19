#include "AppMgr/AppMgr.h"
#include "JSONHandler/JSONHandler.h"
#include "AppMgr/AppFactory.h"
#include "AppMgr/AppLinkInterface.h"
#include "AppMgr/AppMgrCore.h"
#include "AppMgr/RPCAppLinkFactory.h"
#include "AppMgr/RPCBusFactory.h"
#include "AppMgr/AppMgrRegistry.h"

namespace NsAppManager
{
	
AppMgr& AppMgr::getInstance( )
{
	static AppMgr appMgr;
	return appMgr;
}

void AppMgr::setParams(const std::string& address, uint16_t port, std::string name)
{
	AppLinkInterface::setParams(address, port, name);
}

AppMgr::AppMgr()
	:mAppLinkInterface(AppLinkInterface::getInstance())
	,mAppMgrRegistry(AppMgrRegistry::getInstance())
	,mAppMgrCore(AppMgrCore::getInstance())
	,mRPCAppLinkFactory(RPCAppLinkFactory::getInstance())
	,mRPCBusFactory(RPCBusFactory::getInstance())
	,mAppFactory(AppFactory::getInstance())
	,mJSONHandler(0)
{
}

AppMgr::~AppMgr()
{
}

void AppMgr::setJsonHandler(JSONHandler* handler)
{
	mAppMgrCore.setJsonHandler( handler );
}

void AppMgr::onMessageReceivedCallback( MobileRPCMessage * message )
{
	mAppMgrCore.pushMobileRPCMessage( message );
}

/**
 * \brief pure virtual method to process response.
 * \param method method name which has been called.
 * \param root JSON message.
 */
void AppMgr::processResponse(std::string method, Json::Value& root)
{
	mAppLinkInterface.processResponse(method, root);
}

/**
 * \brief pure virtual method to process request.
 * \param root JSON message.
 */
void AppMgr::processRequest(Json::Value& root)
{
	mAppLinkInterface.processRequest(root);
}

/**
 * \brief Process notification message.
 * \brief Notify subscribers about property change.
 * expected notification format example:
 * \code
 * {"jsonrpc": "2.0", "method": "<ComponentName>.<NotificationName>", "params": <list of params>}
 * \endcode
 * \param root JSON message.
 */
void AppMgr::processNotification(Json::Value& root)
{
	mAppLinkInterface.processNotification(root);
}

};
