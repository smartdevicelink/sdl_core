//
// Copyright (c) 2013 Ford Motor Company
//

#include "../include/JSONHandler/SDLRPCObjects/V2/UpdateTurnList_response.h"
#include "UpdateTurnList_responseMarshaller.h"

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
UpdateTurnList_response& UpdateTurnList_response::operator =(const UpdateTurnList_response& c)
{
    success = c.success;
    resultCode = c.resultCode;
    info = c.info ? new std::string(c.info[0]) : 0;

    return *this;
}

UpdateTurnList_response::~UpdateTurnList_response(void)
{}


UpdateTurnList_response::UpdateTurnList_response(const UpdateTurnList_response& c)
{
    *this = c;
}

bool UpdateTurnList_response::checkIntegrity(void)
{
    return UpdateTurnList_responseMarshaller::checkIntegrity(*this);
}

UpdateTurnList_response::UpdateTurnList_response(void) : NsSmartDeviceLinkRPC::SDLRPCResponse(PROTOCOL_VERSION)
{}
