#include "../include/JSONHandler/ALRPCObjects/V2/OnButtonPress.h"
#include "OnButtonPressMarshaller.h"
#include "../include/JSONHandler/ALRPCObjects/V2/Marshaller.h"
#include "ButtonNameMarshaller.h"
#include "ButtonPressModeMarshaller.h"

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

OnButtonPress::~OnButtonPress(void)
{
}


OnButtonPress::OnButtonPress(const OnButtonPress& c) : NsAppLinkRPC::ALRPCMessage(c)
{
  *this=c;
}


bool OnButtonPress::checkIntegrity(void)
{
  return OnButtonPressMarshaller::checkIntegrity(*this);
}


OnButtonPress::OnButtonPress(void) : NsAppLinkRPC::ALRPCMessage(PROTOCOL_VERSION)
{
}



bool OnButtonPress::set_buttonName(const ButtonName& buttonName_)
{
  if(!ButtonNameMarshaller::checkIntegrityConst(buttonName_))   return false;
  buttonName=buttonName_;
  return true;
}

bool OnButtonPress::set_buttonPressMode(const ButtonPressMode& buttonPressMode_)
{
  if(!ButtonPressModeMarshaller::checkIntegrityConst(buttonPressMode_))   return false;
  buttonPressMode=buttonPressMode_;
  return true;
}

bool OnButtonPress::set_customButtonID(unsigned int customButtonID_)
{
  if(customButtonID_>65536)  return false;
  customButtonID=customButtonID_;
  return true;
}




const ButtonName& OnButtonPress::get_buttonName(void) const 
{
  return buttonName;
}

const ButtonPressMode& OnButtonPress::get_buttonPressMode(void) const 
{
  return buttonPressMode;
}

unsigned int OnButtonPress::get_customButtonID(void) const
{
  return customButtonID;
}

