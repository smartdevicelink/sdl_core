#include "../include/JSONHandler/SDLRPCObjects/V2/SetDisplayLayout_request.h"
#include "SetDisplayLayout_requestMarshaller.h"
#include "../include/JSONHandler/SDLRPCObjects/V2/Marshaller.h"

#define PROTOCOL_VERSION	2


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	robok0der
*/

using namespace NsSmartDeviceLinkRPCV2;

SetDisplayLayout_request::~SetDisplayLayout_request(void)
{
}


SetDisplayLayout_request::SetDisplayLayout_request(const SetDisplayLayout_request& c) : NsSmartDeviceLinkRPC::SDLRPCMessage(c)
{
  *this=c;
}


bool SetDisplayLayout_request::checkIntegrity(void)
{
  return SetDisplayLayout_requestMarshaller::checkIntegrity(*this);
}


SetDisplayLayout_request::SetDisplayLayout_request(void) : NsSmartDeviceLinkRPC::SDLRPCMessage(PROTOCOL_VERSION)
{
}



bool SetDisplayLayout_request::set_displayLayout(const std::string& displayLayout_)
{
  displayLayout=displayLayout_;
  return true;
}




const std::string& SetDisplayLayout_request::get_displayLayout(void) const 
{
  return displayLayout;
}

