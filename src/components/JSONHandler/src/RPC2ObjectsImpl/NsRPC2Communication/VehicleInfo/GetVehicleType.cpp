#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/VehicleInfo/GetVehicleType.h"
#include "../src/../include/JSONHandler/RPC2Objects/Marshaller.h"

/*
  interface	NsRPC2Communication::VehicleInfo
  version	1.2
  generated at	Fri Dec 14 06:14:25 2012
  source stamp	Fri Dec 14 06:14:23 2012
  author	robok0der
*/

using namespace NsRPC2Communication::VehicleInfo;


GetVehicleType& GetVehicleType::operator =(const GetVehicleType& c)
{
  appId=c.appId;
  return *this;
}


GetVehicleType::~GetVehicleType(void)
{
}


GetVehicleType::GetVehicleType(void) : 
  RPC2Request(Marshaller::METHOD_NSRPC2COMMUNICATION_VEHICLEINFO__GETVEHICLETYPE)
{
}


GetVehicleType::GetVehicleType(const GetVehicleType& c) : RPC2Request(Marshaller::METHOD_NSRPC2COMMUNICATION_VEHICLEINFO__GETVEHICLETYPE,c.getId())
{
  *this=c;
}


int GetVehicleType::get_appId(void)
{
  return appId;
}

bool GetVehicleType::set_appId(int appId_)
{
  appId=appId_;
  return true;
}

bool GetVehicleType::checkIntegrity(void)
{
  return GetVehicleTypeMarshaller::checkIntegrity(*this);
}
