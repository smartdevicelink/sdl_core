#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/Buttons/OnButtonEvent.h"
#include "../src/../include/JSONHandler/RPC2Objects/Marshaller.h"

/*
  interface	NsRPC2Communication::Buttons
  version	1.2
  generated at	Mon Nov 19 12:18:27 2012
  source stamp	Mon Nov 19 10:17:20 2012
  author	robok0der
*/

using namespace NsRPC2Communication::Buttons;


OnButtonEvent& OnButtonEvent::operator =(const OnButtonEvent& c)
{
  name=c.name;
  mode=c.mode;
  return *this;
}


OnButtonEvent::~OnButtonEvent(void)
{
}


OnButtonEvent::OnButtonEvent(void) : 
  RPC2Notification(Marshaller::METHOD_NSRPC2COMMUNICATION_BUTTONS__ONBUTTONEVENT)
{
}


OnButtonEvent::OnButtonEvent(const OnButtonEvent& c) : RPC2Notification(Marshaller::METHOD_NSRPC2COMMUNICATION_BUTTONS__ONBUTTONEVENT)
{
  *this=c;
}


const NsAppLinkRPC::ButtonName& OnButtonEvent::get_name(void)
{
  return name;
}

bool OnButtonEvent::set_name(const NsAppLinkRPC::ButtonName& name_)
{
  name=name_;
  return true;
}

const NsAppLinkRPC::ButtonEventMode& OnButtonEvent::get_mode(void)
{
  return mode;
}

bool OnButtonEvent::set_mode(const NsAppLinkRPC::ButtonEventMode& mode_)
{
  mode=mode_;
  return true;
}

bool OnButtonEvent::checkIntegrity(void)
{
  return OnButtonEventMarshaller::checkIntegrity(*this);
}
