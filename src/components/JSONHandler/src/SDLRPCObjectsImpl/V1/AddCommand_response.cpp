//
// Copyright (c) 2013 Ford Motor Company
//

#include "../include/JSONHandler/SDLRPCObjects/V1/AddCommand_response.h"
#include "AddCommand_responseMarshaller.h"
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
AddCommand_response& AddCommand_response::operator =(const AddCommand_response& c)
{
    success = c.success;
    resultCode = c.resultCode;
    info = c.info ? new std::string(c.info[0]) : 0;

    return *this;
}

AddCommand_response::~AddCommand_response(void)
{}

AddCommand_response::AddCommand_response(const AddCommand_response& c)
{
    *this = c;
}

bool AddCommand_response::checkIntegrity(void)
{
    return AddCommand_responseMarshaller::checkIntegrity(*this);
}

AddCommand_response::AddCommand_response(void) : SDLRPCResponse(PROTOCOL_VERSION, Marshaller::METHOD_ADDCOMMAND_RESPONSE)
{}
