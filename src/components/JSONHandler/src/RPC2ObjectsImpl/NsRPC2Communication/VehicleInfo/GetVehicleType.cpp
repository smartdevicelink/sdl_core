#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/VehicleInfo/GetVehicleType.h"
#include "../src/../include/JSONHandler/RPC2Objects/Marshaller.h"

/*
  interface	NsRPC2Communication::VehicleInfo
  version	1.2
  generated at	Thu Nov 29 14:32:09 2012
  source stamp	Thu Nov 29 14:32:05 2012
  author	robok0der
*/

using namespace NsRPC2Communication::VehicleInfo;


GetVehicleType& GetVehicleType::operator =(const GetVehicleType& c)
{
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


bool GetVehicleType::checkIntegrity(void)
{
  return GetVehicleTypeMarshaller::checkIntegrity(*this);
}
