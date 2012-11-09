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
    if(mJSONHandler)
    {
        mJSONHandler->sendRPCMessage(message, sessionId);
    }
    else
    {
        LOG4CPLUS_ERROR_EXT(mLogger, " MobileHandler is null!");
    }
}

void MobileHandler::setJsonHandler(JSONHandler *handler)
{
    if(!handler)
    {
        LOG4CPLUS_ERROR_EXT(mLogger, "A null pointer is being assigned - is this the intent?");
        return;
    }
    mJSONHandler = handler;
}

JSONHandler *MobileHandler::getJsonHandler() const
{
    if(!mJSONHandler)
    {
        LOG4CPLUS_ERROR_EXT(mLogger, "JSON Mobile handler hasn't yet been assigned, but an attempt to retrieve it has been made! Face a core dump soon...(((");
    }
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
