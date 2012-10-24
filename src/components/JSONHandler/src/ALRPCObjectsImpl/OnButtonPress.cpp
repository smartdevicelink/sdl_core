#include "../../include/JSONHandler/ALRPCObjects/OnButtonPress.h"
#include "OnButtonPressMarshaller.h"
#include "../../include/JSONHandler/ALRPCObjects/Marshaller.h"
#include "ButtonNameMarshaller.h"
#include "ButtonPressModeMarshaller.h"

#define PROTOCOL_VERSION	1


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/


OnButtonPress::~OnButtonPress(void)
{
}


OnButtonPress::OnButtonPress(const OnButtonPress& c)
{
  *this=c;
}


bool OnButtonPress::checkIntegrity(void)
{
  return OnButtonPressMarshaller::checkIntegrity(*this);
}


OnButtonPress::OnButtonPress(void) : ALRPCNotification(PROTOCOL_VERSION,Marshaller::METHOD_ONBUTTONPRESS)
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

bool OnButtonPress::set_customButtonName(const std::string& customButtonName_)
{
  if(customButtonName_.length()>500)  return false;
  customButtonName=customButtonName_;
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

const std::string& OnButtonPress::get_customButtonName(void) const 
{
  return customButtonName;
}

