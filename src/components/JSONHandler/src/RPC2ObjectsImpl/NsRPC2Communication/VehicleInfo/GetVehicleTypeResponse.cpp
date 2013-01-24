#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/VehicleInfo/GetVehicleTypeResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects/Marshaller.h"

/*
  interface	NsRPC2Communication::VehicleInfo
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	robok0der
*/

using namespace NsRPC2Communication::VehicleInfo;


GetVehicleTypeResponse& GetVehicleTypeResponse::operator =(const GetVehicleTypeResponse& c)
{
  vehicleType=c.vehicleType;
  return *this;
}


GetVehicleTypeResponse::~GetVehicleTypeResponse(void)
{
}


GetVehicleTypeResponse::GetVehicleTypeResponse(void) : 
  RPC2Response(Marshaller::METHOD_NSRPC2COMMUNICATION_VEHICLEINFO__GETVEHICLETYPERESPONSE)
{
}


GetVehicleTypeResponse::GetVehicleTypeResponse(const GetVehicleTypeResponse& c) : RPC2Response(Marshaller::METHOD_NSRPC2COMMUNICATION_VEHICLEINFO__GETVEHICLETYPERESPONSE,c.getId(),c.getResult())
{
  *this=c;
}


const NsAppLinkRPCV2::VehicleType& GetVehicleTypeResponse::get_vehicleType(void)
{
  return vehicleType;
}

bool GetVehicleTypeResponse::set_vehicleType(const NsAppLinkRPCV2::VehicleType& vehicleType_)
{
  vehicleType=vehicleType_;
  return true;
}

bool GetVehicleTypeResponse::checkIntegrity(void)
{
  return GetVehicleTypeResponseMarshaller::checkIntegrity(*this);
}
