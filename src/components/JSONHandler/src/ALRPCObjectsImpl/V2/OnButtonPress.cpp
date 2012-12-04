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
  generated at	Tue Dec  4 14:30:13 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

using namespace NsAppLinkRPCV2;

OnButtonPress::~OnButtonPress(void)
{
}


OnButtonPress::OnButtonPress(const OnButtonPress& c) : ALRPC2Message(c)
{
  *this=c;
}


bool OnButtonPress::checkIntegrity(void)
{
  return OnButtonPressMarshaller::checkIntegrity(*this);
}


OnButtonPress::OnButtonPress(void) : ALRPC2Message(PROTOCOL_VERSION)
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

