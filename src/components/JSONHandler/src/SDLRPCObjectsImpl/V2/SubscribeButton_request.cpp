//
// Copyright (c) 2013 Ford Motor Company
//

#include "../include/JSONHandler/SDLRPCObjects/V2/SubscribeButton_request.h"
#include "SubscribeButton_requestMarshaller.h"
#include "../include/JSONHandler/SDLRPCObjects/V2/Marshaller.h"
#include "ButtonNameMarshaller.h"

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

SubscribeButton_request::~SubscribeButton_request(void)
{
}


SubscribeButton_request::SubscribeButton_request(const SubscribeButton_request& c) : NsSmartDeviceLinkRPC::SDLRPCMessage(c)
{
  *this=c;
}


bool SubscribeButton_request::checkIntegrity(void)
{
  return SubscribeButton_requestMarshaller::checkIntegrity(*this);
}


SubscribeButton_request::SubscribeButton_request(void) : NsSmartDeviceLinkRPC::SDLRPCMessage(PROTOCOL_VERSION)
{
}



bool SubscribeButton_request::set_buttonName(const ButtonName& buttonName_)
{
  if(!ButtonNameMarshaller::checkIntegrityConst(buttonName_))   return false;
  buttonName=buttonName_;
  return true;
}




const ButtonName& SubscribeButton_request::get_buttonName(void) const 
{
  return buttonName;
}

