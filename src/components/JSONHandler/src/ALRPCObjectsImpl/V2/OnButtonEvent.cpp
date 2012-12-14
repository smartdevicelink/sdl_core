#include "../include/JSONHandler/ALRPCObjects/V2/OnButtonEvent.h"
#include "OnButtonEventMarshaller.h"
#include "../include/JSONHandler/ALRPCObjects/V2/Marshaller.h"
#include "ButtonEventModeMarshaller.h"
#include "ButtonNameMarshaller.h"

#define PROTOCOL_VERSION	2


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Dec 13 14:18:29 2012
  source stamp	Thu Dec 13 14:18:27 2012
  author	robok0der
*/

using namespace NsAppLinkRPCV2;

OnButtonEvent::~OnButtonEvent(void)
{
}


OnButtonEvent::OnButtonEvent(const OnButtonEvent& c) : NsAppLinkRPC::ALRPCMessage(c)
{
  *this=c;
}


bool OnButtonEvent::checkIntegrity(void)
{
  return OnButtonEventMarshaller::checkIntegrity(*this);
}


OnButtonEvent::OnButtonEvent(void) : NsAppLinkRPC::ALRPCMessage(PROTOCOL_VERSION)
{
}



bool OnButtonEvent::set_buttonName(const ButtonName& buttonName_)
{
  if(!ButtonNameMarshaller::checkIntegrityConst(buttonName_))   return false;
  buttonName=buttonName_;
  return true;
}

bool OnButtonEvent::set_buttonEventMode(const ButtonEventMode& buttonEventMode_)
{
  if(!ButtonEventModeMarshaller::checkIntegrityConst(buttonEventMode_))   return false;
  buttonEventMode=buttonEventMode_;
  return true;
}

bool OnButtonEvent::set_customButtonID(unsigned int customButtonID_)
{
  if(customButtonID_>65536)  return false;
  customButtonID=customButtonID_;
  return true;
}




const ButtonName& OnButtonEvent::get_buttonName(void) const 
{
  return buttonName;
}

const ButtonEventMode& OnButtonEvent::get_buttonEventMode(void) const 
{
  return buttonEventMode;
}

unsigned int OnButtonEvent::get_customButtonID(void) const
{
  return customButtonID;
}

