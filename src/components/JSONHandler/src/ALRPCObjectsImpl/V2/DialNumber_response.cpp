#include "../include/JSONHandler/ALRPCObjects/V2/DialNumber_response.h"
#include "DialNumber_responseMarshaller.h"

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
DialNumber_response& DialNumber_response::operator =(const DialNumber_response& c)
{
    success = c.success;
    resultCode = c.resultCode;
    info = c.info ? new std::string(c.info[0]) : 0;

    return *this;
}

DialNumber_response::~DialNumber_response(void)
{}

DialNumber_response::DialNumber_response(const DialNumber_response& c)
{
    *this = c;
}

bool DialNumber_response::checkIntegrity(void)
{
    return DialNumber_responseMarshaller::checkIntegrity(*this);
}

DialNumber_response::DialNumber_response(void) : NsAppLinkRPC::ALRPCResponse(PROTOCOL_VERSION)
{}
