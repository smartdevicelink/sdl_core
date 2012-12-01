#include "../include/JSONHandler/ALRPCObjects/GetVehicleData_request.h"
#include "GetVehicleData_requestMarshaller.h"
#include "../include/JSONHandler/ALRPCObjects/Marshaller.h"
#include "VehicleDataTypeMarshaller.h"

#define PROTOCOL_VERSION	42


/*
  interface	Ford Sync RAPI
  version	1.2 / 2.0O
  date		2011-05-17 / 2012-11-02
  generated at	Thu Nov 29 14:49:08 2012
  source stamp	Thu Nov 29 06:50:10 2012
  author	robok0der
*/

using namespace NsAppLinkRPC;

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

