#include "../include/JSONHandler/ALRPCObjects/V1/OnButtonEvent.h"
#include "OnButtonEventMarshaller.h"
#include "../include/JSONHandler/ALRPCObjects/V1/Marshaller.h"
#include "ButtonEventModeMarshaller.h"
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

