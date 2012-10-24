#include "../../include/JSONHandler/ALRPCObjects/OnButtonEvent.h"
#include "OnButtonEventMarshaller.h"
#include "../../include/JSONHandler/ALRPCObjects/Marshaller.h"
#include "ButtonEventModeMarshaller.h"
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


OnButtonEvent::~OnButtonEvent(void)
{
}


OnButtonEvent::OnButtonEvent(const OnButtonEvent& c)
{
  *this=c;
}


bool OnButtonEvent::checkIntegrity(void)
{
  return OnButtonEventMarshaller::checkIntegrity(*this);
}


OnButtonEvent::OnButtonEvent(void) : ALRPCNotification(PROTOCOL_VERSION,Marshaller::METHOD_ONBUTTONEVENT)
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

