#include "../include/JSONHandler/ALRPCObjects/V2/GetVehicleData_request.h"
#include "GetVehicleData_requestMarshaller.h"
#include "../include/JSONHandler/ALRPCObjects/V2/Marshaller.h"
#include "VehicleDataTypeMarshaller.h"

#define PROTOCOL_VERSION	2


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Tue Dec  4 14:30:13 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

using namespace NsAppLinkRPCV2;

GetVehicleData_request::~GetVehicleData_request(void)
{
}


GetVehicleData_request::GetVehicleData_request(const GetVehicleData_request& c) : ALRPC2Message(c)
{
  *this=c;
}


bool GetVehicleData_request::checkIntegrity(void)
{
  return GetVehicleData_requestMarshaller::checkIntegrity(*this);
}


GetVehicleData_request::GetVehicleData_request(void) : ALRPC2Message(PROTOCOL_VERSION)
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

