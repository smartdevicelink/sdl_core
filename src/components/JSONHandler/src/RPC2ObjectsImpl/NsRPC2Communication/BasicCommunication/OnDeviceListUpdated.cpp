#include "../include/JSONHandler/RPC2Objects/NsRPC2Communication/BasicCommunication/OnDeviceListUpdated.h"
#include "../include/JSONHandler/RPC2Objects//Marshaller.h"

/*
  interface	NsRPC2Communication::BasicCommunication
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	robok0der
*/

using namespace NsRPC2Communication::BasicCommunication;


OnDeviceListUpdated& OnDeviceListUpdated::operator =(const OnDeviceListUpdated& c)
{
  if(deviceList)  delete deviceList;
  deviceList= c.deviceList ? new std::vector<std::string>(c.deviceList[0]) : 0;
  return *this;
}


OnDeviceListUpdated::~OnDeviceListUpdated(void)
{
  if(deviceList)  delete deviceList;
}


OnDeviceListUpdated::OnDeviceListUpdated(void) : 
  RPC2Notification(Marshaller::METHOD_NSRPC2COMMUNICATION_BASICCOMMUNICATION__ONDEVICELISTUPDATED),
  deviceList(0)
{
}


OnDeviceListUpdated::OnDeviceListUpdated(const OnDeviceListUpdated& c) : RPC2Notification(Marshaller::METHOD_NSRPC2COMMUNICATION_BASICCOMMUNICATION__ONDEVICELISTUPDATED)
{
  *this=c;
}


const std::vector< std::string>* OnDeviceListUpdated::get_deviceList(void)
{
  return deviceList;
}

bool OnDeviceListUpdated::set_deviceList(const std::vector< std::string>& deviceList_)
{
  if(deviceList)  delete deviceList;
  deviceList=new std::vector< std::string>(deviceList_);
  return true;
}

void OnDeviceListUpdated::reset_deviceList(void)
{
  if(deviceList)  delete deviceList;
  deviceList=0;
}

bool OnDeviceListUpdated::checkIntegrity(void)
{
  return OnDeviceListUpdatedMarshaller::checkIntegrity(*this);
}
