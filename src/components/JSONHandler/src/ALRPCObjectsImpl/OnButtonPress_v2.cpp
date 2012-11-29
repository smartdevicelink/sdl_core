#include "../include/JSONHandler/ALRPCObjects/OnButtonPress_v2.h"
#include "OnButtonPress_v2Marshaller.h"
#include "../include/JSONHandler/ALRPCObjects/Marshaller.h"
#include "ButtonName_v2Marshaller.h"
#include "ButtonPressModeMarshaller.h"

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

OnButtonPress_v2::~OnButtonPress_v2(void)
{
}


OnButtonPress_v2::OnButtonPress_v2(const OnButtonPress_v2& c)
{
  *this=c;
}


bool OnButtonPress_v2::checkIntegrity(void)
{
  return OnButtonPress_v2Marshaller::checkIntegrity(*this);
}


OnButtonPress_v2::OnButtonPress_v2(void) : ALRPCNotification(PROTOCOL_VERSION,Marshaller::METHOD_ONBUTTONPRESS_V2)
{
}



bool OnButtonPress_v2::set_buttonName(const ButtonName_v2& buttonName_)
{
  if(!ButtonName_v2Marshaller::checkIntegrityConst(buttonName_))   return false;
  buttonName=buttonName_;
  return true;
}

bool OnButtonPress_v2::set_buttonPressMode(const ButtonPressMode& buttonPressMode_)
{
  if(!ButtonPressModeMarshaller::checkIntegrityConst(buttonPressMode_))   return false;
  buttonPressMode=buttonPressMode_;
  return true;
}

bool OnButtonPress_v2::set_customButtonName(const std::string& customButtonName_)
{
  if(customButtonName_.length()>500)  return false;
  customButtonName=customButtonName_;
  return true;
}




const ButtonName_v2& OnButtonPress_v2::get_buttonName(void) const 
{
  return buttonName;
}

const ButtonPressMode& OnButtonPress_v2::get_buttonPressMode(void) const 
{
  return buttonPressMode;
}

const std::string& OnButtonPress_v2::get_customButtonName(void) const 
{
  return customButtonName;
}

