#include "../include/JSONHandler/ALRPCObjects/V2/UnsubscribeVehicleData_request.h"
#include "UnsubscribeVehicleData_requestMarshaller.h"
#include "../include/JSONHandler/ALRPCObjects/V2/Marshaller.h"
#include "VehicleDataTypeMarshaller.h"

#define PROTOCOL_VERSION	2


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	robok0der
*/

using namespace NsAppLinkRPCV2;

UnsubscribeVehicleData_request::~UnsubscribeVehicleData_request(void)
{
}


UnsubscribeVehicleData_request::UnsubscribeVehicleData_request(const UnsubscribeVehicleData_request& c) : NsAppLinkRPC::ALRPCMessage(c)
{
  *this=c;
}


bool UnsubscribeVehicleData_request::checkIntegrity(void)
{
  return UnsubscribeVehicleData_requestMarshaller::checkIntegrity(*this);
}


UnsubscribeVehicleData_request::UnsubscribeVehicleData_request(void) : NsAppLinkRPC::ALRPCMessage(PROTOCOL_VERSION)
{
}



bool UnsubscribeVehicleData_request::set_dataType(const std::vector<VehicleDataType>& dataType_)
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




const std::vector<VehicleDataType>& UnsubscribeVehicleData_request::get_dataType(void) const 
{
  return dataType;
}

