//
// Copyright (c) 2013 Ford Motor Company
//

#include "../include/JSONHandler/SDLRPCObjects/V2/SubscribeButton_response.h"
#include "SubscribeButton_responseMarshaller.h"

namespace
{
    const int PROTOCOL_VERSION = 2;
}

/*
  interface Ford Sync RAPI
  version   2.0O
  date      2012-11-02
  generated at  Thu Jan 24 06:36:23 2013
  source stamp  Thu Jan 24 06:35:41 2013
  author    RC
*/

using namespace NsSmartDeviceLinkRPCV2;
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

SubscribeButton_response::SubscribeButton_response(void) : NsSmartDeviceLinkRPC::SDLRPCResponse(PROTOCOL_VERSION)
{}
