#include "../include/JSONHandler/RPC2Objects/Buttons/OnButtonPress.h"
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
  RPC2Notification(Marshaller::METHOD_ONBUTTONPRESS)
{
}


OnButtonPress::OnButtonPress(const OnButtonPress& c) : RPC2Notification(Marshaller::METHOD_ONBUTTONPRESS)
{
  *this=c;
}


const ButtonName& OnButtonPress::get_name(void)
{
  return name;
}

bool OnButtonPress::set_name(const ButtonName& name_)
{
  name=name_;
  return true;
}

const ButtonPressMode& OnButtonPress::get_mode(void)
{
  return mode;
}

bool OnButtonPress::set_mode(const ButtonPressMode& mode_)
{
  mode=mode_;
  return true;
}

bool OnButtonPress::checkIntegrity(void)
{
  return OnButtonPressMarshaller::checkIntegrity(*this);
}
