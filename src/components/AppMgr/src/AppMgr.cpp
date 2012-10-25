#include "AppMgr/AppMgr.h"
#include "JSONHandler/JSONHandler.h"
#include "AppMgr/AppFactory.h"
#include "AppMgr/AppLinkInterface.h"
#include "AppMgr/AppMgrCore.h"
//#include "AppMgr/RPCAppLinkFactory.h"
//#include "AppMgr/RPCBusFactory.h"
#include "AppMgr/AppMgrRegistry.h"
#include "LoggerHelper.hpp"
#include "JSONHandler/ALRPCMessage.h"

namespace NsAppManager
{
log4cplus::Logger AppMgr::mLogger = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("AppMgr"));

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
	,mAppFactory(AppFactory::getInstance())
	,mJSONHandler(0)
{
	LOG4CPLUS_INFO_EXT(mLogger, " AppMgr constructed!");
}

AppMgr::~AppMgr()
{
    LOG4CPLUS_INFO_EXT(mLogger, " AppMgr destructed!");
}

AppMgr::AppMgr(const AppMgr &)
    :mAppLinkInterface(AppLinkInterface::getInstance())
    ,mAppMgrRegistry(AppMgrRegistry::getInstance())
    ,mAppMgrCore(AppMgrCore::getInstance())
    ,mAppFactory(AppFactory::getInstance())
    ,mJSONHandler(0)
{
}

void AppMgr::setJsonHandler(JSONHandler* handler)
{
    mAppMgrCore.setJsonHandler( handler );
}

void AppMgr::setJsonRPC2Handler(JSONRPC2Handler *handler)
{
    mAppMgrCore.setJsonRPC2Handler( handler );
}

void AppMgr::onMessageReceivedCallback(ALRPCMessage * message , unsigned char sessionID)
{
    LOG4CPLUS_INFO_EXT(mLogger, " Message "<<message->getMethodId()<<" received from mobile side");
    mAppMgrCore.pushMobileRPCMessage( message, sessionID );
}

void AppMgr::onCommandReceivedCallback(RPC2Communication::RPC2Command *command)
{
    LOG4CPLUS_INFO_EXT(mLogger, " Message "<<command->getMethod()<<" received from HMI side");
    mAppMgrCore.pushRPC2CommunicationMessage(command);
}

/**
 * \brief pure virtual method to process response.
 * \param method method name which has been called.
 * \param root JSON message.
 */
void AppMgr::processResponse(std::string method, Json::Value& root)
{
	LOG4CPLUS_INFO_EXT(mLogger, " Processing a response to "<<method);
	mAppLinkInterface.processResponse(method, root);
}

/**
 * \brief pure virtual method to process request.
 * \param root JSON message.
 */
void AppMgr::processRequest(Json::Value& root)
{
	LOG4CPLUS_INFO_EXT(mLogger, " Processing a request");
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
	LOG4CPLUS_INFO_EXT(mLogger, " Processing a notification");
	mAppLinkInterface.processNotification(root);
}

const AppLinkInterface& AppMgr::getAppLinkInterface() const
{
	return mAppLinkInterface;
}	

void AppMgr::startAppMgr()
{
    LOG4CPLUS_INFO_EXT(mLogger, " Starting AppMgr...");
	mAppLinkInterface.registerController();
    mAppLinkInterface.prepareComponent();
    LOG4CPLUS_INFO_EXT(mLogger, " Started AppMgr!");
}

};
