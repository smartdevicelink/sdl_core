#include "../include/JSONHandler/ALRPCObjects/OnButtonEvent_v2.h"
#include "OnButtonEvent_v2Marshaller.h"
#include "../include/JSONHandler/ALRPCObjects/Marshaller.h"
#include "ButtonEventModeMarshaller.h"
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

OnButtonEvent_v2::~OnButtonEvent_v2(void)
{
}


OnButtonEvent_v2::OnButtonEvent_v2(const OnButtonEvent_v2& c)
{
  *this=c;
}


bool OnButtonEvent_v2::checkIntegrity(void)
{
  return OnButtonEvent_v2Marshaller::checkIntegrity(*this);
}


OnButtonEvent_v2::OnButtonEvent_v2(void) : ALRPCNotification(PROTOCOL_VERSION,Marshaller::METHOD_ONBUTTONEVENT_V2)
{
}



bool OnButtonEvent_v2::set_buttonName(const ButtonName_v2& buttonName_)
{
  if(!ButtonName_v2Marshaller::checkIntegrityConst(buttonName_))   return false;
  buttonName=buttonName_;
  return true;
}

bool OnButtonEvent_v2::set_buttonEventMode(const ButtonEventMode& buttonEventMode_)
{
  if(!ButtonEventModeMarshaller::checkIntegrityConst(buttonEventMode_))   return false;
  buttonEventMode=buttonEventMode_;
  return true;
}

bool OnButtonEvent_v2::set_customButtonID(unsigned int customButtonID_)
{
  if(customButtonID_>65536)  return false;
  customButtonID=customButtonID_;
  return true;
}




const ButtonName_v2& OnButtonEvent_v2::get_buttonName(void) const 
{
  return buttonName;
}

const ButtonEventMode& OnButtonEvent_v2::get_buttonEventMode(void) const 
{
  return buttonEventMode;
}

unsigned int OnButtonEvent_v2::get_customButtonID(void) const
{
  return customButtonID;
}

