#include "../../include/JSONHandler/ALRPCObjects/OnButtonPress.h"
#include "OnButtonPressMarshaller.h"
#include "../../include/JSONHandler/ALRPCObjects/Marshaller.h"
#include "ButtonNameMarshaller.h"
#include "ButtonPressModeMarshaller.h"

#define PROTOCOL_VERSION	1


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Fri Oct 26 06:31:48 2012
  source stamp	Thu Oct 25 06:49:27 2012
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




const ButtonName& OnButtonPress::get_buttonName(void) const 
{
  return buttonName;
}

const ButtonPressMode& OnButtonPress::get_buttonPressMode(void) const 
{
  return buttonPressMode;
}

