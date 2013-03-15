//
// Copyright (c) 2013 Ford Motor Company
//

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/Buttons/OnButtonPress.h"
#include "../src/../include/JSONHandler/RPC2Objects/Marshaller.h"

/*
  interface	NsRPC2Communication::Buttons
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	RC
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


const NsSmartDeviceLinkRPCV2::ButtonName& OnButtonPress::get_name(void)
{
  return name;
}

bool OnButtonPress::set_name(const NsSmartDeviceLinkRPCV2::ButtonName& name_)
{
  name=name_;
  return true;
}

const NsSmartDeviceLinkRPCV2::ButtonPressMode& OnButtonPress::get_mode(void)
{
  return mode;
}

bool OnButtonPress::set_mode(const NsSmartDeviceLinkRPCV2::ButtonPressMode& mode_)
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
