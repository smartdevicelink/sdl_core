//
// Copyright (c) 2013 Ford Motor Company
//

#include "../include/JSONHandler/SDLRPCObjects/V2/GetVehicleData_request.h"
#include "GetVehicleData_requestMarshaller.h"
#include "../include/JSONHandler/SDLRPCObjects/V2/Marshaller.h"
#include "VehicleDataTypeMarshaller.h"

#define PROTOCOL_VERSION	2


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	RC
*/

using namespace NsSmartDeviceLinkRPCV2;

GetVehicleData_request::~GetVehicleData_request(void)
{
}


GetVehicleData_request::GetVehicleData_request(const GetVehicleData_request& c) : NsSmartDeviceLinkRPC::SDLRPCMessage(c)
{
  *this=c;
}


bool GetVehicleData_request::checkIntegrity(void)
{
  return GetVehicleData_requestMarshaller::checkIntegrity(*this);
}


GetVehicleData_request::GetVehicleData_request(void) : NsSmartDeviceLinkRPC::SDLRPCMessage(PROTOCOL_VERSION)
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

