#include "../include/JSONHandler/ALRPCObjects/V2/AlertManeuver_response.h"
#include "AlertManeuver_responseMarshaller.h"

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
AlertManeuver_response& AlertManeuver_response::operator =(const AlertManeuver_response& c)
{
    success = c.success;
    resultCode = c.resultCode;
    info = c.info ? new std::string(c.info[0]) : 0;

    return *this;
}

AlertManeuver_response::~AlertManeuver_response(void)
{}


AlertManeuver_response::AlertManeuver_response(const AlertManeuver_response& c)
{
    *this = c;
}

bool AlertManeuver_response::checkIntegrity(void)
{
    return AlertManeuver_responseMarshaller::checkIntegrity(*this);
}

AlertManeuver_response::AlertManeuver_response(void) : NsAppLinkRPC::ALRPCResponse(PROTOCOL_VERSION)
{}
