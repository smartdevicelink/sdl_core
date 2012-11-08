#include "AppMgr/HMIHandler.h"
#include "JSONHandler/JSONRPC2Handler.h"

namespace NsAppManager
{

HMIHandler& HMIHandler::getInstance()
{
    static HMIHandler instance;
    return instance;
}

void HMIHandler::setReadyState(bool ready)
{
    m_bHMIReady = ready;
}

void HMIHandler::sendNotification(const RPC2Communication::RPC2Notification *command)
{
    if(m_bHMIReady)
    {
        mJSONRPC2Handler->sendNotification(command);
    }
}

void HMIHandler::sendResponse(const RPC2Communication::RPC2Response *command)
{
    if(m_bHMIReady)
    {
        mJSONRPC2Handler->sendResponse(command);
    }
}

void HMIHandler::sendRequest(const RPC2Communication::RPC2Request *command)
{
    if(m_bHMIReady)
    {
        mJSONRPC2Handler->sendRequest(command);
    }
}

void HMIHandler::setJsonRPC2Handler(JSONRPC2Handler *handler)
{
    mJSONRPC2Handler = handler;
}

JSONRPC2Handler *HMIHandler::getJsonRPC2Handler() const
{
    return mJSONRPC2Handler;
}

HMIHandler::HMIHandler()
    :mJSONRPC2Handler(0)
    ,m_bHMIReady(false)
{
}

HMIHandler::HMIHandler(const HMIHandler &)
    :mJSONRPC2Handler(0)
    ,m_bHMIReady(false)
{
}

}
