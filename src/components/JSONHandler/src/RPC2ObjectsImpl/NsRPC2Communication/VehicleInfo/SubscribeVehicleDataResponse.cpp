#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/VehicleInfo/SubscribeVehicleDataResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects/Marshaller.h"

/*
  interface	NsRPC2Communication::VehicleInfo
  version	1.2
  generated at	Fri Dec 14 06:14:25 2012
  source stamp	Fri Dec 14 06:14:23 2012
  author	robok0der
*/

using namespace NsRPC2Communication::VehicleInfo;


SubscribeVehicleDataResponse& SubscribeVehicleDataResponse::operator =(const SubscribeVehicleDataResponse& c)
{
  if(dataResult)  delete dataResult;
  dataResult= c.dataResult ? new std::vector<NsAppLinkRPCV2::VehicleDataResult>(c.dataResult[0]) : 0;
  return *this;
}


SubscribeVehicleDataResponse::~SubscribeVehicleDataResponse(void)
{
  if(dataResult)  delete dataResult;
}


SubscribeVehicleDataResponse::SubscribeVehicleDataResponse(void) : 
  RPC2Response(Marshaller::METHOD_NSRPC2COMMUNICATION_VEHICLEINFO__SUBSCRIBEVEHICLEDATARESPONSE),
  dataResult(0)
{
}


SubscribeVehicleDataResponse::SubscribeVehicleDataResponse(const SubscribeVehicleDataResponse& c) : RPC2Response(Marshaller::METHOD_NSRPC2COMMUNICATION_VEHICLEINFO__SUBSCRIBEVEHICLEDATARESPONSE,c.getId(),c.getResult())
{
  *this=c;
}


const std::vector< NsAppLinkRPCV2::VehicleDataResult>* SubscribeVehicleDataResponse::get_dataResult(void)
{
  return dataResult;
}

bool SubscribeVehicleDataResponse::set_dataResult(const std::vector< NsAppLinkRPCV2::VehicleDataResult>& dataResult_)
{
  if(dataResult)  delete dataResult;
  dataResult=new std::vector< NsAppLinkRPCV2::VehicleDataResult>(dataResult_);
  return true;
}

void SubscribeVehicleDataResponse::reset_dataResult(void)
{
  if(dataResult)  delete dataResult;
  dataResult=0;
}

bool SubscribeVehicleDataResponse::checkIntegrity(void)
{
  return SubscribeVehicleDataResponseMarshaller::checkIntegrity(*this);
}
