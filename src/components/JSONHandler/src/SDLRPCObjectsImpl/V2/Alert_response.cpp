#include "../include/JSONHandler/SDLRPCObjects/V2/Alert_response.h"
#include "Alert_responseMarshaller.h"

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
Alert_response& Alert_response::operator =(const Alert_response& c)
{
    success = c.success;
    resultCode = c.resultCode;
    info = c.info ? new std::string(c.info[0]) : 0;
    tryAgainTime = c.tryAgainTime;

    return *this;
}


Alert_response::~Alert_response(void)
{}

Alert_response::Alert_response(const Alert_response& c)
{
    *this = c;
}

bool Alert_response::checkIntegrity(void)
{
    return Alert_responseMarshaller::checkIntegrity(*this);
}

Alert_response::Alert_response(void)
    : NsSmartDeviceLinkRPC::SDLRPCResponse(PROTOCOL_VERSION)
    , tryAgainTime(0)
{}

bool Alert_response::set_tryAgainTime(unsigned int tryAgainTime_)
{
    if (tryAgainTime_ > 2000000000)
    {
        return false;
    }

    tryAgainTime = tryAgainTime_;
    return true;
}

unsigned int Alert_response::get_tryAgainTime(void) const
{
    return tryAgainTime;
}
