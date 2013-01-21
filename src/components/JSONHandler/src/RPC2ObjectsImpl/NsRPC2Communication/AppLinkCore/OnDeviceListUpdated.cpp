#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/AppLinkCore/OnDeviceListUpdated.h"
#include "../src/../include/JSONHandler/RPC2Objects//Marshaller.h"

/*
  interface	NsRPC2Communication::AppLinkCore
  version	1.2
  generated at	Fri Dec 14 06:14:25 2012
  source stamp	Fri Dec 14 06:14:23 2012
  author	robok0der
*/

using namespace NsRPC2Communication::AppLinkCore;


OnDeviceListUpdated& OnDeviceListUpdated::operator =(const OnDeviceListUpdated& c)
{
  if (deviceList)
  {
    delete deviceList;
  }
  if (c.deviceList)
  {
    deviceList=new std::vector<std::string> (*c.deviceList);  
  }
  return *this;
}


OnDeviceListUpdated::~OnDeviceListUpdated(void)
{
  if (deviceList)
  {
    delete deviceList;
    deviceList = 0;
  }
}


OnDeviceListUpdated::OnDeviceListUpdated(void) : 
  RPC2Notification(Marshaller::METHOD_NSRPC2COMMUNICATION_APPLINKCORE__ONDEVICELISTUPDATED)
  ,deviceList(0)
{
}


OnDeviceListUpdated::OnDeviceListUpdated(const OnDeviceListUpdated& c) : RPC2Notification(Marshaller::METHOD_NSRPC2COMMUNICATION_APPLINKCORE__ONDEVICELISTUPDATED)
,deviceList(0)
{
  *this=c;
}


const std::vector< std::string>* OnDeviceListUpdated::get_deviceList(void)
{
  return deviceList;
}

bool OnDeviceListUpdated::set_deviceList(const std::vector< std::string>& deviceList_)
{
  if (deviceList)
  {
    delete deviceList;
  }
  deviceList=new std::vector<std::string> (deviceList_);
  return true;
}

bool OnDeviceListUpdated::checkIntegrity(void)
{
  return OnDeviceListUpdatedMarshaller::checkIntegrity(*this);
}
