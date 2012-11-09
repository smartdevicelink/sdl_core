#include "AppMgr/MobileHandler.h"
#include "JSONHandler/JSONHandler.h"
#include "LoggerHelper.hpp"

namespace NsAppManager
{

log4cplus::Logger MobileHandler::mLogger = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("AppMgrCore"));

MobileHandler &MobileHandler::getInstance()
{
    static MobileHandler instance;
    return instance;
}

void MobileHandler::sendRPCMessage(const AppLinkRPC::ALRPCMessage *message, unsigned char sessionId)
{
    mJSONHandler->sendRPCMessage(message, sessionId);
}

void MobileHandler::setJsonHandler(JSONHandler *handler)
{
    mJSONHandler = handler;
}

JSONHandler *MobileHandler::getJsonHandler() const
{
    return mJSONHandler;
}

MobileHandler::MobileHandler()
    :mJSONHandler(0)
{
}

MobileHandler::MobileHandler(const MobileHandler &)
    :mJSONHandler(0)
{
}

}
