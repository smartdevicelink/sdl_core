#include "../include/JSONHandler/SDLRPCObjects/V2/DeleteSubMenu_response.h"
#include "DeleteSubMenu_responseMarshaller.h"

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
  author    robok0der
*/

using namespace NsSmartDeviceLinkRPCV2;
DeleteSubMenu_response& DeleteSubMenu_response::operator =(const DeleteSubMenu_response& c)
{
    success = c.success;
    resultCode = c.resultCode;
    info = c.info ? new std::string(c.info[0]) : 0;

    return *this;
}

DeleteSubMenu_response::~DeleteSubMenu_response(void)
{}

DeleteSubMenu_response::DeleteSubMenu_response(const DeleteSubMenu_response& c)
{
    *this = c;
}

bool DeleteSubMenu_response::checkIntegrity(void)
{
    return DeleteSubMenu_responseMarshaller::checkIntegrity(*this);
}

DeleteSubMenu_response::DeleteSubMenu_response(void) : NsSmartDeviceLinkRPC::SDLRPCResponse(PROTOCOL_VERSION)
{}
