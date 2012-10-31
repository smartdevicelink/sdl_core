#include "AppMgr/SubscribeButton.h"
#include "AppMgr/AppMgrCore.h"
#include "JSONHandler/ALRPCObjects/SubscribeButton_request.h"
#include "JSONHandler/ALRPCObjects/SubscribeButton_response.h"
#include "JSONHandler/ALRPCObjects/ButtonName.h"
#include "LoggerHelper.hpp"

namespace NsAppManager
{

SubscribeButtonCmd::SubscribeButtonCmd(const RegistryItem *receiver, const void *params)
    :IAppCommand(receiver, params)
{
}

void SubscribeButtonCmd::execute()
{
    LOG4CPLUS_INFO_EXT(mLogger, " A SubscribeButtonCmd command has been executing");
    Message* msg = (Message*)mParams;
    SubscribeButton_request * object = (SubscribeButton_request*)msg->first;
    AppMgrCore::getInstance().subscribeButton( *msg );
    SubscribeButton_response* response = new SubscribeButton_response();
    response->setCorrelationID(object->getCorrelationID());
    response->setMessageType(ALRPCMessage::RESPONSE);
    response->set_success(true);
    response->set_resultCode(Result::SUCCESS);
    Message responseMessage = Message(response, msg->second);
    AppMgrCore::getInstance().sendMobileRPCResponse( responseMessage );
}

}
