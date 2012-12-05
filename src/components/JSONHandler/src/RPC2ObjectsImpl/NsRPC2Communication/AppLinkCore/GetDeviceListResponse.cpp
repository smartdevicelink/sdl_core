#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/AppLinkCore/GetDeviceListResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects//Marshaller.h"

/*
  interface	NsRPC2Communication::AppLinkCore
  version	1.2
  generated at	Tue Dec  4 16:38:13 2012
  source stamp	Tue Dec  4 16:37:04 2012
  author	robok0der
*/

using namespace NsRPC2Communication::AppLinkCore;


GetDeviceListResponse& GetDeviceListResponse::operator =(const GetDeviceListResponse& c)
{
  deviceList=c.deviceList;
  return *this;
}


GetDeviceListResponse::~GetDeviceListResponse(void)
{
}


GetDeviceListResponse::GetDeviceListResponse(void) : 
  RPC2Response(Marshaller::METHOD_NSRPC2COMMUNICATION_APPLINKCORE__GETDEVICELISTRESPONSE)
{
}


GetDeviceListResponse::GetDeviceListResponse(const GetDeviceListResponse& c) : RPC2Response(Marshaller::METHOD_NSRPC2COMMUNICATION_APPLINKCORE__GETDEVICELISTRESPONSE,c.getId(),c.getResult())
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
