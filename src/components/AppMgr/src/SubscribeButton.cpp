#include "AppMgr/SubscribeButton.h"
#include "AppMgr/AppMgrCore.h"
#include "AppMgr/AppMgrRegistry.h"
#include "JSONHandler/ALRPCObjects/SubscribeButton_request.h"
#include "JSONHandler/ALRPCObjects/SubscribeButton_response.h"
#include "JSONHandler/ALRPCObjects/ButtonName.h"
#include "JSONHandler/JSONHandler.h"
#include "LoggerHelper.hpp"

namespace NsAppManager
{

SubscribeButtonCmd::SubscribeButtonCmd(const RegistryItem *receiver, const void *params)
    :IAppCommand(receiver, params)
{
    if(!receiver || !params)
    {
        LOG4CPLUS_ERROR_EXT(mLogger, " Check params for null!");
        return;
    }
}

void SubscribeButtonCmd::execute()
{
    LOG4CPLUS_INFO_EXT(mLogger, " A SubscribeButtonCmd command has been executing");
    Message* msg = (Message*)mParams;
    if(!mParams)
    {
        LOG4CPLUS_ERROR_EXT(mLogger, " No params supplied in constructor, null pointer exception is about to occur!");
        return;
    }
    SubscribeButton_request * object = (SubscribeButton_request*)msg->first;
    RegistryItem* item = AppMgrRegistry::getInstance().getItem(msg->second);
    AppMgrCore::getInstance().mButtonsMapping.addButton( object->get_buttonName(), item );
    SubscribeButton_response* response = new SubscribeButton_response();
    response->setCorrelationID(object->getCorrelationID());
    response->setMessageType(ALRPCMessage::RESPONSE);
    response->set_success(true);
    response->set_resultCode(Result::SUCCESS);
    AppMgrCore::getInstance().mJSONHandler->sendRPCMessage(response, msg->second);
}

}
