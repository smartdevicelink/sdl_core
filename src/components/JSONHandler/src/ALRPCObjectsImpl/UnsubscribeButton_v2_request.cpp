#include "../include/JSONHandler/ALRPCObjects/UnsubscribeButton_v2_request.h"
#include "UnsubscribeButton_v2_requestMarshaller.h"
#include "../include/JSONHandler/ALRPCObjects/Marshaller.h"
#include "ButtonName_v2Marshaller.h"

#define PROTOCOL_VERSION	42


/*
  interface	Ford Sync RAPI
  version	1.2 / 2.0O
  date		2011-05-17 / 2012-11-02
  generated at	Thu Nov 29 14:49:08 2012
  source stamp	Thu Nov 29 06:50:10 2012
  author	robok0der
*/

using namespace NsAppLinkRPC;

UnsubscribeButton_v2_request::~UnsubscribeButton_v2_request(void)
{
}


UnsubscribeButton_v2_request::UnsubscribeButton_v2_request(const UnsubscribeButton_v2_request& c)
{
  *this=c;
}


bool UnsubscribeButton_v2_request::checkIntegrity(void)
{
  return UnsubscribeButton_v2_requestMarshaller::checkIntegrity(*this);
}


UnsubscribeButton_v2_request::UnsubscribeButton_v2_request(void) : ALRPCRequest(PROTOCOL_VERSION,Marshaller::METHOD_UNSUBSCRIBEBUTTON_V2_REQUEST)
{
}



bool UnsubscribeButton_v2_request::set_buttonName(const ButtonName_v2& buttonName_)
{
  if(!ButtonName_v2Marshaller::checkIntegrityConst(buttonName_))   return false;
  buttonName=buttonName_;
  return true;
}




const ButtonName_v2& UnsubscribeButton_v2_request::get_buttonName(void) const 
{
  return buttonName;
}

