//
// Copyright (c) 2013 Ford Motor Company
//

#include "../include/JSONHandler/SDLRPCObjects/V1/Alert_response.h"
#include "Alert_responseMarshaller.h"
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
Alert_response& Alert_response::operator =(const Alert_response& c)
{
    success = c.success;
    resultCode = c.resultCode;
    info = c.info ? new std::string(c.info[0]) : 0;

    return *this;
}

Alert_response::~Alert_response(void)
{}

Alert_response::Alert_response(const Alert_response& c)
{
    *this = c;
}

bool Alert_response::checkIntegrity(void)
{
    return Alert_responseMarshaller::checkIntegrity(*this);
}

Alert_response::Alert_response(void) : SDLRPCResponse(PROTOCOL_VERSION, Marshaller::METHOD_ALERT_RESPONSE)
{}
