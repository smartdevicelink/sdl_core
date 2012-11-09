#include "AppMgr/HMIHandler.h"
#include "JSONHandler/JSONRPC2Handler.h"
#include "LoggerHelper.hpp"

namespace NsAppManager
{

log4cplus::Logger HMIHandler::mLogger = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("AppMgrCore"));

HMIHandler& HMIHandler::getInstance()
{
    static HMIHandler instance;
    return instance;
}

void HMIHandler::setReadyState(bool ready)
{
    if(mJSONRPC2Handler)
    {
        m_bHMIReady = ready;
    }
    else
    {
        LOG4CPLUS_ERROR_EXT(mLogger, " HMIHandler is about to set ready, but is null!");
    }
}

void HMIHandler::sendNotification(const RPC2Communication::RPC2Notification *command)
{
    if(m_bHMIReady)
    {
        if(mJSONRPC2Handler)
        {
            LOG4CPLUS_INFO_EXT(mLogger, " Sending a notification "<<command->getMethod());
            mJSONRPC2Handler->sendNotification(command);
        }
    }
    else
    {
        LOG4CPLUS_ERROR_EXT(mLogger, " HMIHandler not ready yet!");
    }
}

void HMIHandler::sendResponse(const RPC2Communication::RPC2Response *command)
{
    if(m_bHMIReady)
    {
        if(mJSONRPC2Handler)
        {
            LOG4CPLUS_INFO_EXT(mLogger, " Sending a response "<<command->getMethod());
            mJSONRPC2Handler->sendResponse(command);
        }
    }
    else
    {
        LOG4CPLUS_ERROR_EXT(mLogger, " HMIHandler not ready yet!");
    }
}

void HMIHandler::sendRequest(const RPC2Communication::RPC2Request *command)
{
    if(m_bHMIReady)
    {
        if(mJSONRPC2Handler)
        {
            LOG4CPLUS_INFO_EXT(mLogger, " Sending a request "<<command->getMethod());
            mJSONRPC2Handler->sendRequest(command);
        }
    }
    else
    {
        LOG4CPLUS_ERROR_EXT(mLogger, " HMIHandler not ready yet!");
    }
}

void HMIHandler::setJsonRPC2Handler(JSONRPC2Handler *handler)
{
    if(!handler)
    {
        LOG4CPLUS_ERROR_EXT(mLogger, "A null pointer is being assigned - is this the intent?");
        return;
    }
    mJSONRPC2Handler = handler;
}

JSONRPC2Handler *HMIHandler::getJsonRPC2Handler() const
{
    if(!mJSONRPC2Handler)
    {
        LOG4CPLUS_ERROR_EXT(mLogger, "JSON RPC2 handler hasn't yet been assigned, but an attempt to retrieve it has been made! Face a core dump soon...(((");
    }
    return mJSONRPC2Handler;
}

HMIHandler::HMIHandler()
    :mJSONRPC2Handler(0)
    ,m_bHMIReady(false)
{
    LOG4CPLUS_INFO_EXT(mLogger, " HMIHandler constructed!");
}

HMIHandler::HMIHandler(const HMIHandler &)
    :mJSONRPC2Handler(0)
    ,m_bHMIReady(false)
{
}

}
