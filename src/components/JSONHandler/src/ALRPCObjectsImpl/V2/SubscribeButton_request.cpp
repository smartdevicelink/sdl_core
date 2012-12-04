#include "../include/JSONHandler/ALRPCObjects/V2/SubscribeButton_request.h"
#include "SubscribeButton_requestMarshaller.h"
#include "../include/JSONHandler/ALRPCObjects/V2/Marshaller.h"
#include "ButtonNameMarshaller.h"

#define PROTOCOL_VERSION	2


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Tue Dec  4 14:30:13 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

using namespace NsAppLinkRPCV2;

SubscribeButton_request::~SubscribeButton_request(void)
{
}


SubscribeButton_request::SubscribeButton_request(const SubscribeButton_request& c) : ALRPC2Message(c)
{
  *this=c;
}


bool SubscribeButton_request::checkIntegrity(void)
{
  return SubscribeButton_requestMarshaller::checkIntegrity(*this);
}


SubscribeButton_request::SubscribeButton_request(void) : ALRPC2Message(PROTOCOL_VERSION)
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

