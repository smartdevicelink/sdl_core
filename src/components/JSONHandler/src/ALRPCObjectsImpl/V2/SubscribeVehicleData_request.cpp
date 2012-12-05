#include "../include/JSONHandler/ALRPCObjects/V2/SubscribeVehicleData_request.h"
#include "SubscribeVehicleData_requestMarshaller.h"
#include "../include/JSONHandler/ALRPCObjects/V2/Marshaller.h"
#include "VehicleDataTypeMarshaller.h"

#define PROTOCOL_VERSION	2


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Tue Dec  4 17:03:13 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

using namespace NsAppLinkRPCV2;

SubscribeVehicleData_request::~SubscribeVehicleData_request(void)
{
}


SubscribeVehicleData_request::SubscribeVehicleData_request(const SubscribeVehicleData_request& c) : NsAppLinkRPC::ALRPCMessage(c)
{
  *this=c;
}


bool SubscribeVehicleData_request::checkIntegrity(void)
{
  return SubscribeVehicleData_requestMarshaller::checkIntegrity(*this);
}


SubscribeVehicleData_request::SubscribeVehicleData_request(void) : NsAppLinkRPC::ALRPCMessage(PROTOCOL_VERSION)
{
}



bool SubscribeVehicleData_request::set_dataType(const std::vector<VehicleDataType>& dataType_)
{
  unsigned int i=dataType_.size();
  if(i>100 || i<1)  return false;
  while(i--)
  {
    if(!VehicleDataTypeMarshaller::checkIntegrityConst(dataType_[i]))   return false;
  }
  dataType=dataType_;
  return true;
}




const std::vector<VehicleDataType>& SubscribeVehicleData_request::get_dataType(void) const 
{
  return dataType;
}

