#include "../include/JSONHandler/ALRPCObjects/OnButtonEvent.h"
#include "OnButtonEventMarshaller.h"
#include "../include/JSONHandler/ALRPCObjects/Marshaller.h"
#include "ButtonEventModeMarshaller.h"
#include "ButtonNameMarshaller.h"

#define PROTOCOL_VERSION	1


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Wed Nov  7 13:10:41 2012
  source stamp	Thu Oct 25 06:49:27 2012
  author	robok0der
*/

using namespace AppLinkRPC;

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




const ButtonName& OnButtonEvent::get_buttonName(void) const 
{
  return buttonName;
}

const ButtonEventMode& OnButtonEvent::get_buttonEventMode(void) const 
{
  return buttonEventMode;
}

