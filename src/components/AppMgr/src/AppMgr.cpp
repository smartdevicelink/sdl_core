#include "AppMgr/AppMgr.h"
#include "JSONHandler/JSONHandler.h"
#include "AppMgr/AppMgrCore.h"
#include "AppMgr/AppMgrRegistry.h"
#include "LoggerHelper.hpp"
#include "JSONHandler/ALRPCMessage.h"

namespace NsAppManager
{
log4cplus::Logger AppMgr::mLogger = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("AppMgr"));

/**
 * \brief Returning class instance
 * \return class instance
 */
AppMgr& AppMgr::getInstance( )
{
	static AppMgr appMgr;
	return appMgr;
}

/**
 * \brief Default class constructor
 */
AppMgr::AppMgr()
    :mAppMgrRegistry(AppMgrRegistry::getInstance())
	,mAppMgrCore(AppMgrCore::getInstance())
	,mJSONHandler(0)
{
	LOG4CPLUS_INFO_EXT(mLogger, " AppMgr constructed!");
}

/**
 * \brief Default class destructor
 */
AppMgr::~AppMgr()
{
    LOG4CPLUS_INFO_EXT(mLogger, " AppMgr destructed!");
}

/**
 * \brief Copy constructor
 */
AppMgr::AppMgr(const AppMgr &)
    :mAppMgrRegistry(AppMgrRegistry::getInstance())
    ,mAppMgrCore(AppMgrCore::getInstance())
    ,mJSONHandler(0)
{
}

/**
 * \brief Sets Json mobile handler instance
 * \param handler Json mobile handler
 */
void AppMgr::setJsonHandler(JSONHandler* handler)
{
    if(!handler)
    {
        LOG4CPLUS_ERROR_EXT(mLogger, " Setting null handler!");
        return;
    }
    mAppMgrCore.setJsonHandler( handler );
}

/**
 * \brief Sets Json RPC2 handler instance
 * \param handler Json RPC2 handler
 */
void AppMgr::setJsonRPC2Handler(JSONRPC2Handler *handler)
{
    if(!handler)
    {
        LOG4CPLUS_ERROR_EXT(mLogger, " Setting null handler!");
        return;
    }
    mAppMgrCore.setJsonRPC2Handler( handler );
}

/**
 * \brief callback to proceed received mobile message
 * \param message the received message
 * \param sessionID an id of a session associated with the application which sends message
 */
void AppMgr::onMessageReceivedCallback(NsAppLinkRPC::ALRPCMessage * message , unsigned char sessionID)
{
    if(!message)
    {
        LOG4CPLUS_ERROR_EXT(mLogger, " Calling a function with null command! Session id "<<sessionID);
        return;
    }
    LOG4CPLUS_INFO_EXT(mLogger, " Message "<<message->getMethodId()<<" received from mobile side");
    mAppMgrCore.pushMobileRPCMessage( message, sessionID );
}

/**
 * \brief callback to proceed received RPC2 command
 * \param command the received command
 */
void AppMgr::onCommandReceivedCallback(RPC2Communication::RPC2Command *command)
{
    if(!command)
    {
        LOG4CPLUS_ERROR_EXT(mLogger, " Calling a function with null command!");
        return;
    }
    LOG4CPLUS_INFO_EXT(mLogger, " Message "<<command->getMethod()<<" received from HMI side");
    mAppMgrCore.pushRPC2CommunicationMessage(command);
}

/**
 * \brief method to process response.
 * \param method method name which has been called.
 * \param root JSON message.
 */
void AppMgr::processResponse(std::string method, Json::Value& root)
{
	LOG4CPLUS_INFO_EXT(mLogger, " Processing a response to "<<method);
    //mAppLinkInterface.processResponse(method, root);
}

/**
 * \brief method to process request.
 * \param root JSON message.
 */
void AppMgr::processRequest(Json::Value& root)
{
	LOG4CPLUS_INFO_EXT(mLogger, " Processing a request");
    //mAppLinkInterface.processRequest(root);
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
    //mAppLinkInterface.processNotification(root);
}

/**
 * \brief method to execute threads.
 */
void AppMgr::executeThreads()
{
    LOG4CPLUS_INFO_EXT(mLogger, " Threads are being started!");

    mAppMgrCore.executeThreads();

    LOG4CPLUS_INFO_EXT(mLogger, " Threads have been started!");
}

}
