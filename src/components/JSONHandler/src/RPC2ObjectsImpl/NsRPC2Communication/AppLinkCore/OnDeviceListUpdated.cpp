#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/AppLinkCore/OnDeviceListUpdated.h"
#include "../src/../include/JSONHandler/RPC2Objects//Marshaller.h"

/*
  interface	NsRPC2Communication::AppLinkCore
  version	1.2
  generated at	Tue Dec  4 16:38:13 2012
  source stamp	Tue Dec  4 16:37:04 2012
  author	robok0der
*/

using namespace NsRPC2Communication::AppLinkCore;


OnDeviceListUpdated& OnDeviceListUpdated::operator =(const OnDeviceListUpdated& c)
{
  deviceList=c.deviceList;
  return *this;
}


OnDeviceListUpdated::~OnDeviceListUpdated(void)
{
}


OnDeviceListUpdated::OnDeviceListUpdated(void) : 
  RPC2Notification(Marshaller::METHOD_NSRPC2COMMUNICATION_APPLINKCORE__ONDEVICELISTUPDATED)
{
}


OnDeviceListUpdated::OnDeviceListUpdated(const OnDeviceListUpdated& c) : RPC2Notification(Marshaller::METHOD_NSRPC2COMMUNICATION_APPLINKCORE__ONDEVICELISTUPDATED)
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
