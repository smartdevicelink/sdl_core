#include "../include/JSONHandler/SDLRPCObjects/V1/SubscribeButton_response.h"
#include "SubscribeButton_responseMarshaller.h"
#include "../include/JSONHandler/SDLRPCObjects/V1/Marshaller.h"

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

using namespace NsSmartDeviceLinkRPC;
SubscribeButton_response& SubscribeButton_response::operator =(const SubscribeButton_response& c)
{
    success = c.success;
    resultCode = c.resultCode;
    info = c.info ? new std::string(c.info[0]) : 0;

    return *this;
}

SubscribeButton_response::~SubscribeButton_response(void)
{}

SubscribeButton_response::SubscribeButton_response(const SubscribeButton_response& c)
{
    *this = c;
}

bool SubscribeButton_response::checkIntegrity(void)
{
    return SubscribeButton_responseMarshaller::checkIntegrity(*this);
}

SubscribeButton_response::SubscribeButton_response(void)
    : SDLRPCResponse(PROTOCOL_VERSION, Marshaller::METHOD_SUBSCRIBEBUTTON_RESPONSE)
{}
