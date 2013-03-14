#include "../include/JSONHandler/SDLRPCObjects/V2/Speak_response.h"
#include "Speak_responseMarshaller.h"

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
Speak_response& Speak_response::operator =(const Speak_response& c)
{
    success = c.success;
    resultCode = c.resultCode;
    info = c.info ? new std::string(c.info[0]) : 0;

    return *this;
}

Speak_response::~Speak_response(void)
{}

Speak_response::Speak_response(const Speak_response& c)
{
    *this = c;
}

bool Speak_response::checkIntegrity(void)
{
    return Speak_responseMarshaller::checkIntegrity(*this);
}

Speak_response::Speak_response(void) : NsSmartDeviceLinkRPC::SDLRPCResponse(PROTOCOL_VERSION)
{}
