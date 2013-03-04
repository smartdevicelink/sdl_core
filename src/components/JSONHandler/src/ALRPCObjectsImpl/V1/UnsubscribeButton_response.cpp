#include "../include/JSONHandler/ALRPCObjects/V1/UnsubscribeButton_response.h"
#include "UnsubscribeButton_responseMarshaller.h"
#include "../include/JSONHandler/ALRPCObjects/V1/Marshaller.h"

namespace
{
    const int PROTOCOL_VERSION = 1;
}

/*
  interface Ford Sync RAPI
  version   1.2
  date      2011-05-17
  generated at  Thu Jan 24 06:36:21 2013
  source stamp  Thu Jan 24 06:35:34 2013
  author    robok0der
*/

using namespace NsAppLinkRPC;
UnsubscribeButton_response& UnsubscribeButton_response::operator =(const UnsubscribeButton_response& c)
{
    success = c.success;
    resultCode = c.resultCode;
    info = c.info ? new std::string(c.info[0]) : 0;

    return *this;
}

UnsubscribeButton_response::~UnsubscribeButton_response(void)
{}

UnsubscribeButton_response::UnsubscribeButton_response(const UnsubscribeButton_response& c)
{
    *this = c;
}

bool UnsubscribeButton_response::checkIntegrity(void)
{
    return UnsubscribeButton_responseMarshaller::checkIntegrity(*this);
}

UnsubscribeButton_response::UnsubscribeButton_response(void)
    : ALRPCResponse(PROTOCOL_VERSION, Marshaller::METHOD_UNSUBSCRIBEBUTTON_RESPONSE)
{}
