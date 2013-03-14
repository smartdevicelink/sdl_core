#include "../include/JSONHandler/SDLRPCObjects/V2/ChangeRegistration_response.h"
#include "ChangeRegistration_responseMarshaller.h"

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
ChangeRegistration_response& ChangeRegistration_response::operator =(const ChangeRegistration_response& c)
{
    success = c.success;
    resultCode = c.resultCode;
    info = c.info ? new std::string(c.info[0]) : 0;

    return *this;
}

ChangeRegistration_response::~ChangeRegistration_response(void)
{}

ChangeRegistration_response::ChangeRegistration_response(const ChangeRegistration_response& c)
{
    *this = c;
}

bool ChangeRegistration_response::checkIntegrity(void)
{
    return ChangeRegistration_responseMarshaller::checkIntegrity(*this);
}


ChangeRegistration_response::ChangeRegistration_response(void) : NsSmartDeviceLinkRPC::SDLRPCResponse(PROTOCOL_VERSION)
{}
