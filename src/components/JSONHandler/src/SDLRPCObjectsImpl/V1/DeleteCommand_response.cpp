//
// Copyright (c) 2013 Ford Motor Company
//

#include "../include/JSONHandler/SDLRPCObjects/V1/DeleteCommand_response.h"
#include "DeleteCommand_responseMarshaller.h"
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
DeleteCommand_response& DeleteCommand_response::operator =(const DeleteCommand_response& c)
{
    success = c.success;
    resultCode = c.resultCode;
    info = c.info ? new std::string(c.info[0]) : 0;

    return *this;
}

DeleteCommand_response::~DeleteCommand_response(void)
{}

DeleteCommand_response::DeleteCommand_response(const DeleteCommand_response& c)
{
    *this = c;
}

bool DeleteCommand_response::checkIntegrity(void)
{
    return DeleteCommand_responseMarshaller::checkIntegrity(*this);
}

DeleteCommand_response::DeleteCommand_response(void)
    : SDLRPCResponse(PROTOCOL_VERSION, Marshaller::METHOD_DELETECOMMAND_RESPONSE)
{}
