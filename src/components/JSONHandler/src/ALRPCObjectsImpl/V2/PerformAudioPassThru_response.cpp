#include "../include/JSONHandler/ALRPCObjects/V2/PerformAudioPassThru_response.h"
#include "PerformAudioPassThru_responseMarshaller.h"

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
PerformAudioPassThru_response& PerformAudioPassThru_response::operator =(const PerformAudioPassThru_response& c)
{
    success = c.success;
    resultCode = c.resultCode;
    info = c.info ? new std::string(c.info[0]) : 0;

    return *this;
}

PerformAudioPassThru_response::~PerformAudioPassThru_response(void)
{}

PerformAudioPassThru_response::PerformAudioPassThru_response(const PerformAudioPassThru_response& c)
{
    *this = c;
}

bool PerformAudioPassThru_response::checkIntegrity(void)
{
    return PerformAudioPassThru_responseMarshaller::checkIntegrity(*this);
}

PerformAudioPassThru_response::PerformAudioPassThru_response(void) : NsAppLinkRPC::ALRPCResponse(PROTOCOL_VERSION)
{}
