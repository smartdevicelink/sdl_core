#include "../include/JSONHandler/ALRPCObjects/V2/ScrollableMessage_response.h"
#include "ScrollableMessage_responseMarshaller.h"

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

ScrollableMessage_response& ScrollableMessage_response::operator =(const ScrollableMessage_response& c)
{
    success = c.success;
    resultCode = c.resultCode;
    info = c.info ? new std::string(c.info[0]) : 0;

    return *this;
}

ScrollableMessage_response::~ScrollableMessage_response(void)
{}

ScrollableMessage_response::ScrollableMessage_response(const ScrollableMessage_response& c)
{
    *this = c;
}

bool ScrollableMessage_response::checkIntegrity(void)
{
    return ScrollableMessage_responseMarshaller::checkIntegrity(*this);
}

ScrollableMessage_response::ScrollableMessage_response(void) : NsAppLinkRPC::ALRPCResponse(PROTOCOL_VERSION)
{}
