//
// Copyright (c) 2013 Ford Motor Company
//

#include "../include/JSONHandler/SDLRPCObjects/V2/DialNumber_request.h"
#include "DialNumber_requestMarshaller.h"
#include "../include/JSONHandler/SDLRPCObjects/V2/Marshaller.h"

#define PROTOCOL_VERSION	2


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	RC
*/

using namespace NsSmartDeviceLinkRPCV2;

DialNumber_request::~DialNumber_request(void)
{
}


DialNumber_request::DialNumber_request(const DialNumber_request& c) : NsSmartDeviceLinkRPC::SDLRPCMessage(c)
{
  *this=c;
}


bool DialNumber_request::checkIntegrity(void)
{
  return DialNumber_requestMarshaller::checkIntegrity(*this);
}


DialNumber_request::DialNumber_request(void) : NsSmartDeviceLinkRPC::SDLRPCMessage(PROTOCOL_VERSION)
{
}



bool DialNumber_request::set_number(const std::string& number_)
{
  if(number_.length()>500)  return false;
  number=number_;
  return true;
}




const std::string& DialNumber_request::get_number(void) const 
{
  return number;
}

