//
// Copyright (c) 2013 Ford Motor Company
//

#include "../include/JSONHandler/SDLRPCObjects/V1/EncodedSyncPData_response.h"
#include "EncodedSyncPData_responseMarshaller.h"
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
EncodedSyncPData_response& EncodedSyncPData_response::operator =(const EncodedSyncPData_response& c)
{
    success = c.success;
    resultCode = c.resultCode;
    info = c.info ? new std::string(c.info[0]) : 0;

    return *this;
}

EncodedSyncPData_response::~EncodedSyncPData_response(void)
{}

EncodedSyncPData_response::EncodedSyncPData_response(const EncodedSyncPData_response& c)
{
    *this = c;
}

bool EncodedSyncPData_response::checkIntegrity(void)
{
    return EncodedSyncPData_responseMarshaller::checkIntegrity(*this);
}

EncodedSyncPData_response::EncodedSyncPData_response(void)
    : SDLRPCResponse(PROTOCOL_VERSION, Marshaller::METHOD_ENCODEDSYNCPDATA_RESPONSE)
{}
