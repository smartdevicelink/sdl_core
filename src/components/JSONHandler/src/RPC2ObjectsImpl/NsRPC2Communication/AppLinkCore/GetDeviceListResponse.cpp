#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/AppLinkCore/GetDeviceListResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects//Marshaller.h"

/*
  interface	NsRPC2Communication::AppLinkCore
  version	1.2
  generated at	Fri Dec 14 06:14:25 2012
  source stamp	Fri Dec 14 06:14:23 2012
  author	robok0der
*/

using namespace NsRPC2Communication::AppLinkCore;


GetDeviceListResponse& GetDeviceListResponse::operator =(const GetDeviceListResponse& c)
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


GetDeviceListResponse::~GetDeviceListResponse(void)
{
  if (deviceList)
  {
    delete deviceList;
    deviceList = 0;
  }
}


GetDeviceListResponse::GetDeviceListResponse(void) : 
  RPC2Response(Marshaller::METHOD_NSRPC2COMMUNICATION_APPLINKCORE__GETDEVICELISTRESPONSE)
  ,deviceList(0)
{
}


GetDeviceListResponse::GetDeviceListResponse(const GetDeviceListResponse& c) : RPC2Response(Marshaller::METHOD_NSRPC2COMMUNICATION_APPLINKCORE__GETDEVICELISTRESPONSE,c.getId(),c.getResult())
,deviceList(0)
{
  *this=c;
}


const std::vector< std::string>* GetDeviceListResponse::get_deviceList(void)
{
  return deviceList;
}

bool GetDeviceListResponse::set_deviceList(const std::vector< std::string>& deviceList_)
{
  if (deviceList)
  {
    delete deviceList;
  }
  deviceList=new std::vector<std::string> (deviceList_);
  return true;
}

bool GetDeviceListResponse::checkIntegrity(void)
{
  return GetDeviceListResponseMarshaller::checkIntegrity(*this);
}
