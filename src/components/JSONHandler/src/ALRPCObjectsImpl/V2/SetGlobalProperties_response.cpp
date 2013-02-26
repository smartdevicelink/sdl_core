#include "../include/JSONHandler/ALRPCObjects/V2/SetGlobalProperties_response.h"
#include "SetGlobalProperties_responseMarshaller.h"

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

using namespace NsAppLinkRPCV2;
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

SetGlobalProperties_response::SetGlobalProperties_response(void) : NsAppLinkRPC::ALRPCResponse(PROTOCOL_VERSION)
{}
