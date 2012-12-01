#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/OnDeviceListUpdated.h"
#include "../src/../include/JSONHandler/RPC2Objects/Marshaller.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Thu Nov 29 14:32:09 2012
  source stamp	Thu Nov 29 14:32:05 2012
  author	robok0der
*/

using namespace NsRPC2Communication::UI;


OnDeviceListUpdated& OnDeviceListUpdated::operator =(const OnDeviceListUpdated& c)
{
  deviceList=c.deviceList;
  return *this;
}


OnDeviceListUpdated::~OnDeviceListUpdated(void)
{
}


OnDeviceListUpdated::OnDeviceListUpdated(void) : 
  RPC2Notification(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__ONDEVICELISTUPDATED)
{
}


OnDeviceListUpdated::OnDeviceListUpdated(const OnDeviceListUpdated& c) : RPC2Notification(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__ONDEVICELISTUPDATED)
{
  *this=c;
}


const std::vector< std::string>& OnDeviceListUpdated::get_deviceList(void)
{
  return deviceList;
}

bool OnDeviceListUpdated::set_deviceList(const std::vector< std::string>& deviceList_)
{
  deviceList=deviceList_;
  return true;
}

bool OnDeviceListUpdated::checkIntegrity(void)
{
  return OnDeviceListUpdatedMarshaller::checkIntegrity(*this);
}
