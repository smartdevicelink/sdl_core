#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/Buttons/OnButtonPress.h"
#include "../src/../include/JSONHandler/RPC2Objects/Marshaller.h"

/*
  interface	NsRPC2Communication::Buttons
  version	1.2
  generated at	Tue Dec  4 16:38:13 2012
  source stamp	Tue Dec  4 16:37:04 2012
  author	robok0der
*/

using namespace NsRPC2Communication::Buttons;


OnButtonPress& OnButtonPress::operator =(const OnButtonPress& c)
{
  name=c.name;
  mode=c.mode;
  if(customButtonName)  delete customButtonName;
  customButtonName= c.customButtonName ? new std::string(c.customButtonName[0]) : 0;
  return *this;
}


OnButtonPress::~OnButtonPress(void)
{
  if(customButtonName)  delete customButtonName;
}


OnButtonPress::OnButtonPress(void) : 
  RPC2Notification(Marshaller::METHOD_NSRPC2COMMUNICATION_BUTTONS__ONBUTTONPRESS),
  customButtonName(0)
{
}


OnButtonPress::OnButtonPress(const OnButtonPress& c) : RPC2Notification(Marshaller::METHOD_NSRPC2COMMUNICATION_BUTTONS__ONBUTTONPRESS)
{
  *this=c;
}


const NsAppLinkRPC::ButtonName& OnButtonPress::get_name(void)
{
  return name;
}

bool OnButtonPress::set_name(const NsAppLinkRPC::ButtonName& name_)
{
  name=name_;
  return true;
}

const NsAppLinkRPC::ButtonPressMode& OnButtonPress::get_mode(void)
{
  return mode;
}

bool OnButtonPress::set_mode(const NsAppLinkRPC::ButtonPressMode& mode_)
{
  mode=mode_;
  return true;
}

const std::string* OnButtonPress::get_customButtonName(void)
{
  return customButtonName;
}

bool OnButtonPress::set_customButtonName(const std::string& customButtonName_)
{
  if(customButtonName)  delete customButtonName;
  customButtonName=new std::string(customButtonName_);
  return true;
}

void OnButtonPress::reset_customButtonName(void)
{
  if(customButtonName)  delete customButtonName;
  customButtonName=0;
}

bool OnButtonPress::checkIntegrity(void)
{
  return OnButtonPressMarshaller::checkIntegrity(*this);
}
