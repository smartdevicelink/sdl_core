#include "../include/JSONHandler/SDLRPCObjects/V2/UnsubscribeButton_response.h"
#include "UnsubscribeButton_responseMarshaller.h"

namespace
{
    const int PROTOCOL_VERSION = 2;
}

/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	robok0der
*/

using namespace NsSmartDeviceLinkRPCV2;
UnsubscribeButton_response& UnsubscribeButton_response::operator =(const UnsubscribeButton_response& c)
{
  success= c.success;
  resultCode= c.resultCode;
  info= c.info ? new std::string(c.info[0]) : 0;

  return *this;
}

UnsubscribeButton_response::~UnsubscribeButton_response(void)
{}


UnsubscribeButton_response::UnsubscribeButton_response(const UnsubscribeButton_response& c)
{
  *this=c;
}

bool UnsubscribeButton_response::checkIntegrity(void)
{
  return UnsubscribeButton_responseMarshaller::checkIntegrity(*this);
}

UnsubscribeButton_response::UnsubscribeButton_response(void) : NsSmartDeviceLinkRPC::SDLRPCResponse(PROTOCOL_VERSION)
{}
