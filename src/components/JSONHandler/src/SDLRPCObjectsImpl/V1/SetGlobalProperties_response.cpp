#include "../include/JSONHandler/SDLRPCObjects/V1/SetGlobalProperties_response.h"
#include "SetGlobalProperties_responseMarshaller.h"
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
SetGlobalProperties_response& SetGlobalProperties_response::operator =(const SetGlobalProperties_response& c)
{
    success = c.success;
    resultCode = c.resultCode;
    info = c.info ? new std::string(c.info[0]) : 0;

    return *this;
}

SetGlobalProperties_response::~SetGlobalProperties_response(void)
{}

SetGlobalProperties_response::SetGlobalProperties_response(const SetGlobalProperties_response& c)
{
    *this = c;
}

bool SetGlobalProperties_response::checkIntegrity(void)
{
    return SetGlobalProperties_responseMarshaller::checkIntegrity(*this);
}

SetGlobalProperties_response::SetGlobalProperties_response(void)
    : SDLRPCResponse(PROTOCOL_VERSION, Marshaller::METHOD_SETGLOBALPROPERTIES_RESPONSE)
{
}
