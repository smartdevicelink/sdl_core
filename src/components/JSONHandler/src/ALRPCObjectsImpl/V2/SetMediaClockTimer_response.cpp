#include "../include/JSONHandler/ALRPCObjects/V2/SetMediaClockTimer_response.h"
#include "SetMediaClockTimer_responseMarshaller.h"

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
SetMediaClockTimer_response& SetMediaClockTimer_response::operator =(const SetMediaClockTimer_response& c)
{
    success = c.success;
    resultCode = c.resultCode;
    info = c.info ? new std::string(c.info[0]) : 0;

    return *this;
}

SetMediaClockTimer_response::~SetMediaClockTimer_response(void)
{}

SetMediaClockTimer_response::SetMediaClockTimer_response(const SetMediaClockTimer_response& c)
{
    *this = c;
}

bool SetMediaClockTimer_response::checkIntegrity(void)
{
    return SetMediaClockTimer_responseMarshaller::checkIntegrity(*this);
}

SetMediaClockTimer_response::SetMediaClockTimer_response(void) : NsAppLinkRPC::ALRPCResponse(PROTOCOL_VERSION)
{}
