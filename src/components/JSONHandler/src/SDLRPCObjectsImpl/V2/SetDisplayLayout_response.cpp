#include "../include/JSONHandler/SDLRPCObjects/V2/SetDisplayLayout_response.h"
#include "SetDisplayLayout_responseMarshaller.h"

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
SetDisplayLayout_response& SetDisplayLayout_response::operator =(const SetDisplayLayout_response& c)
{
    success = c.success;
    resultCode = c.resultCode;
    info = c.info ? new std::string(c.info[0]) : 0;

    return *this;
}


SetDisplayLayout_response::~SetDisplayLayout_response(void)
{}


SetDisplayLayout_response::SetDisplayLayout_response(const SetDisplayLayout_response& c)
{
    *this = c;
}

bool SetDisplayLayout_response::checkIntegrity(void)
{
    return SetDisplayLayout_responseMarshaller::checkIntegrity(*this);
}

SetDisplayLayout_response::SetDisplayLayout_response(void) : NsSmartDeviceLinkRPC::SDLRPCResponse(PROTOCOL_VERSION)
{}
