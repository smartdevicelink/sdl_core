#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/Buttons/OnButtonPress.h"
#include "../src/../include/JSONHandler/RPC2Objects/Marshaller.h"

/*
  interface	NsRPC2Communication::Buttons
  version	1.2
  generated at	Fri Dec 14 06:14:25 2012
  source stamp	Fri Dec 14 06:14:23 2012
  author	robok0der
*/

using namespace NsRPC2Communication::Buttons;


OnButtonPress& OnButtonPress::operator =(const OnButtonPress& c)
{
  name=c.name;
  mode=c.mode;
  if(customButtonID)  delete customButtonID;
  customButtonID= c.customButtonID ? new unsigned int(c.customButtonID[0]) : 0;
  return *this;
}


OnButtonPress::~OnButtonPress(void)
{
  if(customButtonID)  delete customButtonID;
}


OnButtonPress::OnButtonPress(void) : 
  RPC2Notification(Marshaller::METHOD_NSRPC2COMMUNICATION_BUTTONS__ONBUTTONPRESS),
  customButtonID(0)
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

const unsigned int* OnButtonPress::get_customButtonID(void)
{
  return customButtonID;
}

bool OnButtonPress::set_customButtonID(const unsigned int& customButtonID_)
{
  if(customButtonID)  delete customButtonID;
  customButtonID=new unsigned int(customButtonID_);
  return true;
}

void OnButtonPress::reset_customButtonID(void)
{
  if(customButtonID)  delete customButtonID;
  customButtonID=0;
}

bool OnButtonPress::checkIntegrity(void)
{
  return OnButtonPressMarshaller::checkIntegrity(*this);
}
