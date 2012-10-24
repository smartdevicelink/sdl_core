#include "../../include/JSONHandler/ALRPCObjects/SubscribeButton_request.h"
#include "SubscribeButton_requestMarshaller.h"
#include "../../include/JSONHandler/ALRPCObjects/Marshaller.h"
#include "ButtonNameMarshaller.h"

#define PROTOCOL_VERSION	1


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/


SubscribeButton_request::~SubscribeButton_request(void)
{
}


SubscribeButton_request::SubscribeButton_request(const SubscribeButton_request& c)
{
  *this=c;
}


bool SubscribeButton_request::checkIntegrity(void)
{
  return SubscribeButton_requestMarshaller::checkIntegrity(*this);
}


SubscribeButton_request::SubscribeButton_request(void) : ALRPCRequest(PROTOCOL_VERSION,Marshaller::METHOD_SUBSCRIBEBUTTON_REQUEST)
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

