//
// Copyright (c) 2013 Ford Motor Company
//

#include "../include/JSONHandler/SDLRPCObjects/V1/Show_response.h"
#include "Show_responseMarshaller.h"
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
  author    RC
*/

using namespace NsSmartDeviceLinkRPC;
Show_response& Show_response::operator =(const Show_response& c)
{
    success = c.success;
    resultCode = c.resultCode;
    info = c.info ? new std::string(c.info[0]) : 0;

    return *this;
}

Show_response::~Show_response(void)
{}

Show_response::Show_response(const Show_response& c)
{
    *this = c;
}

bool Show_response::checkIntegrity(void)
{
    return Show_responseMarshaller::checkIntegrity(*this);
}

Show_response::Show_response(void) : SDLRPCResponse(PROTOCOL_VERSION, Marshaller::METHOD_SHOW_RESPONSE)
{}
