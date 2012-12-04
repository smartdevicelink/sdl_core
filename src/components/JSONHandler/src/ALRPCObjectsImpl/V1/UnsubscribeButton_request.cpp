#include "../include/JSONHandler/ALRPCObjects/V1/UnsubscribeButton_request.h"
#include "UnsubscribeButton_requestMarshaller.h"
#include "../include/JSONHandler/ALRPCObjects/V1/Marshaller.h"
#include "ButtonNameMarshaller.h"

#define PROTOCOL_VERSION	1


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Tue Dec  4 16:02:39 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

using namespace NsAppLinkRPC;

UnsubscribeButton_request::~UnsubscribeButton_request(void)
{
}


UnsubscribeButton_request::UnsubscribeButton_request(const UnsubscribeButton_request& c)
{
  *this=c;
}


bool UnsubscribeButton_request::checkIntegrity(void)
{
  return UnsubscribeButton_requestMarshaller::checkIntegrity(*this);
}


UnsubscribeButton_request::UnsubscribeButton_request(void) : ALRPCRequest(PROTOCOL_VERSION,Marshaller::METHOD_UNSUBSCRIBEBUTTON_REQUEST)
{
}



bool UnsubscribeButton_request::set_buttonName(const ButtonName& buttonName_)
{
  if(!ButtonNameMarshaller::checkIntegrityConst(buttonName_))   return false;
  buttonName=buttonName_;
  return true;
}




const ButtonName& UnsubscribeButton_request::get_buttonName(void) const 
{
  return buttonName;
}

