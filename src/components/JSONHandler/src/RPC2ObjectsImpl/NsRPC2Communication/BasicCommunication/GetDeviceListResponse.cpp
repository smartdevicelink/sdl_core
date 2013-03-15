//
// Copyright (c) 2013 Ford Motor Company
//

#include "../include/JSONHandler/RPC2Objects/NsRPC2Communication/BasicCommunication/GetDeviceListResponse.h"
#include "../include/JSONHandler/RPC2Objects//Marshaller.h"

/*
  interface	NsRPC2Communication::BasicCommunication
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	RC
*/

using namespace NsRPC2Communication::BasicCommunication;


GetDeviceListResponse& GetDeviceListResponse::operator =(const GetDeviceListResponse& c)
{
  if(deviceList)  delete deviceList;
  deviceList= c.deviceList ? new std::vector<std::string>(c.deviceList[0]) : 0;
  return *this;
}


GetDeviceListResponse::~GetDeviceListResponse(void)
{
  if(deviceList)  delete deviceList;
}


GetDeviceListResponse::GetDeviceListResponse(void) : 
  RPC2Response(Marshaller::METHOD_NSRPC2COMMUNICATION_BASICCOMMUNICATION__GETDEVICELISTRESPONSE),
  deviceList(0)
{
}


GetDeviceListResponse::GetDeviceListResponse(const GetDeviceListResponse& c) : RPC2Response(Marshaller::METHOD_NSRPC2COMMUNICATION_BASICCOMMUNICATION__GETDEVICELISTRESPONSE,c.getId(),c.getResult())
{
  *this=c;
}


const std::vector< std::string>* GetDeviceListResponse::get_deviceList(void)
{
  return deviceList;
}

bool GetDeviceListResponse::set_deviceList(const std::vector< std::string>& deviceList_)
{
  if(deviceList)  delete deviceList;
  deviceList=new std::vector< std::string>(deviceList_);
  return true;
}

void GetDeviceListResponse::reset_deviceList(void)
{
  if(deviceList)  delete deviceList;
  deviceList=0;
}

bool GetDeviceListResponse::checkIntegrity(void)
{
  return GetDeviceListResponseMarshaller::checkIntegrity(*this);
}
