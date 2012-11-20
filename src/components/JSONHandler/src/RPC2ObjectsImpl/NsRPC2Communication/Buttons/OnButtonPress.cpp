#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/Buttons/OnButtonPress.h"
#include "../src/../include/JSONHandler/RPC2Objects/Marshaller.h"

/*
  interface	NsRPC2Communication::Buttons
  version	1.2
  generated at	Tue Nov 20 13:32:23 2012
  source stamp	Mon Nov 19 10:17:20 2012
  author	robok0der
*/

using namespace NsRPC2Communication::Buttons;


OnButtonPress& OnButtonPress::operator =(const OnButtonPress& c)
{
  name=c.name;
  mode=c.mode;
  return *this;
}


OnButtonPress::~OnButtonPress(void)
{
}


OnButtonPress::OnButtonPress(void) : 
  RPC2Notification(Marshaller::METHOD_NSRPC2COMMUNICATION_BUTTONS__ONBUTTONPRESS)
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

bool OnButtonPress::checkIntegrity(void)
{
  return OnButtonPressMarshaller::checkIntegrity(*this);
}
