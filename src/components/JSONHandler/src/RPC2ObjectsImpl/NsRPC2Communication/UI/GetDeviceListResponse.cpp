#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/GetDeviceListResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects/Marshaller.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Tue Dec  4 15:06:30 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

using namespace NsRPC2Communication::UI;


GetDeviceListResponse& GetDeviceListResponse::operator =(const GetDeviceListResponse& c)
{
  deviceList=c.deviceList;
  return *this;
}


GetDeviceListResponse::~GetDeviceListResponse(void)
{
}


GetDeviceListResponse::GetDeviceListResponse(void) : 
  RPC2Response(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__GETDEVICELISTRESPONSE)
{
}


GetDeviceListResponse::GetDeviceListResponse(const GetDeviceListResponse& c) : RPC2Response(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__GETDEVICELISTRESPONSE,c.getId(),c.getResult())
{
  *this=c;
}


const std::vector< std::string>& GetDeviceListResponse::get_deviceList(void)
{
  return deviceList;
}

bool GetDeviceListResponse::set_deviceList(const std::vector< std::string>& deviceList_)
{
  deviceList=deviceList_;
  return true;
}

bool GetDeviceListResponse::checkIntegrity(void)
{
  return GetDeviceListResponseMarshaller::checkIntegrity(*this);
}
