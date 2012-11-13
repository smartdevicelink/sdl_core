#include "../include/JSONHandler/RPC2Objects/Buttons/OnButtonEvent.h"
#include "../include/JSONHandler/RPC2Objects/Buttons/Marshaller.h"

/*
  interface	RPC2Communication::Buttons
  version	1.2
  generated at	Wed Nov  7 11:25:50 2012
  source stamp	Wed Nov  7 09:31:20 2012
  author	robok0der
*/

using namespace RPC2Communication;
using namespace AppLinkRPC;
using namespace RPC2Communication::Buttons;


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
  RPC2Notification(Marshaller::METHOD_ONBUTTONEVENT)
{
}


OnButtonEvent::OnButtonEvent(const OnButtonEvent& c) : RPC2Notification(Marshaller::METHOD_ONBUTTONEVENT)
{
  *this=c;
}


const ButtonName& OnButtonEvent::get_name(void)
{
  return name;
}

bool OnButtonEvent::set_name(const ButtonName& name_)
{
  name=name_;
  return true;
}

const ButtonEventMode& OnButtonEvent::get_mode(void)
{
  return mode;
}

bool OnButtonEvent::set_mode(const ButtonEventMode& mode_)
{
  mode=mode_;
  return true;
}

bool OnButtonEvent::checkIntegrity(void)
{
  return OnButtonEventMarshaller::checkIntegrity(*this);
}
