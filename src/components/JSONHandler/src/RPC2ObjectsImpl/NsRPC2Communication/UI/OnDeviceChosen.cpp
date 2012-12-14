#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/OnDeviceChosen.h"
#include "../src/../include/JSONHandler/RPC2Objects/Marshaller.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Fri Dec 14 06:14:25 2012
  source stamp	Fri Dec 14 06:14:23 2012
  author	robok0der
*/

using namespace NsRPC2Communication::UI;


OnDeviceChosen& OnDeviceChosen::operator =(const OnDeviceChosen& c)
{
  deviceName=c.deviceName;
  return *this;
}


OnDeviceChosen::~OnDeviceChosen(void)
{
}


OnDeviceChosen::OnDeviceChosen(void) : 
  RPC2Notification(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__ONDEVICECHOSEN)
{
}


OnDeviceChosen::OnDeviceChosen(const OnDeviceChosen& c) : RPC2Notification(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__ONDEVICECHOSEN)
{
  *this=c;
}


const std::string& OnDeviceChosen::get_deviceName(void)
{
  return deviceName;
}

bool OnDeviceChosen::set_deviceName(const std::string& deviceName_)
{
  deviceName=deviceName_;
  return true;
}

bool OnDeviceChosen::checkIntegrity(void)
{
  return OnDeviceChosenMarshaller::checkIntegrity(*this);
}
