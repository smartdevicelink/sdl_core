#include "../../include/JSONHandler/ALRPCObjects/GetVehicleData_request.h"
#include "GetVehicleData_requestMarshaller.h"
#include "../../include/JSONHandler/ALRPCObjects/Marshaller.h"
#include "VehicleDataTypeMarshaller.h"

#define PROTOCOL_VERSION	1


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/


GetVehicleData_request::~GetVehicleData_request(void)
{
}


GetVehicleData_request::GetVehicleData_request(const GetVehicleData_request& c)
{
  *this=c;
}


bool GetVehicleData_request::checkIntegrity(void)
{
  return GetVehicleData_requestMarshaller::checkIntegrity(*this);
}


GetVehicleData_request::GetVehicleData_request(void) : ALRPCRequest(PROTOCOL_VERSION,Marshaller::METHOD_GETVEHICLEDATA_REQUEST)
{
}



bool GetVehicleData_request::set_dataType(const VehicleDataType& dataType_)
{
  if(!VehicleDataTypeMarshaller::checkIntegrityConst(dataType_))   return false;
  dataType=dataType_;
  return true;
}




const VehicleDataType& GetVehicleData_request::get_dataType(void) const 
{
  return dataType;
}

