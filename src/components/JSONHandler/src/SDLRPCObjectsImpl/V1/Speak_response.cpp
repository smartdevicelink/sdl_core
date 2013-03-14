#include "../include/JSONHandler/SDLRPCObjects/V1/Speak_response.h"
#include "Speak_responseMarshaller.h"
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

Speak_response::Speak_response(void) : SDLRPCResponse(PROTOCOL_VERSION, Marshaller::METHOD_SPEAK_RESPONSE)
{}
