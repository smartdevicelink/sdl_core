//
// Copyright (c) 2013 Ford Motor Company
//

#include "../include/JSONHandler/SDLRPCObjects/V1/AddSubMenu_response.h"
#include "AddSubMenu_responseMarshaller.h"
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
AddSubMenu_response& AddSubMenu_response::operator =(const AddSubMenu_response& c)
{
    success = c.success;
    resultCode = c.resultCode;
    info = c.info ? new std::string(c.info[0]) : 0;

    return *this;
}

AddSubMenu_response::~AddSubMenu_response(void)
{}

AddSubMenu_response::AddSubMenu_response(const AddSubMenu_response& c)
{
    *this = c;
}

bool AddSubMenu_response::checkIntegrity(void)
{
    return AddSubMenu_responseMarshaller::checkIntegrity(*this);
}

AddSubMenu_response::AddSubMenu_response(void) : SDLRPCResponse(PROTOCOL_VERSION, Marshaller::METHOD_ADDSUBMENU_RESPONSE)
{}
