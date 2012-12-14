#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/VehicleInfo/UnsubscribeVehicleDataResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects/Marshaller.h"

/*
  interface	NsRPC2Communication::VehicleInfo
  version	1.2
  generated at	Fri Dec 14 06:14:25 2012
  source stamp	Fri Dec 14 06:14:23 2012
  author	robok0der
*/

using namespace NsRPC2Communication::VehicleInfo;


UnsubscribeVehicleDataResponse& UnsubscribeVehicleDataResponse::operator =(const UnsubscribeVehicleDataResponse& c)
{
  if(dataResult)  delete dataResult;
  dataResult= c.dataResult ? new std::vector<NsAppLinkRPCV2::VehicleDataResult>(c.dataResult[0]) : 0;
  return *this;
}


UnsubscribeVehicleDataResponse::~UnsubscribeVehicleDataResponse(void)
{
  if(dataResult)  delete dataResult;
}


UnsubscribeVehicleDataResponse::UnsubscribeVehicleDataResponse(void) : 
  RPC2Response(Marshaller::METHOD_NSRPC2COMMUNICATION_VEHICLEINFO__UNSUBSCRIBEVEHICLEDATARESPONSE),
  dataResult(0)
{
}


UnsubscribeVehicleDataResponse::UnsubscribeVehicleDataResponse(const UnsubscribeVehicleDataResponse& c) : RPC2Response(Marshaller::METHOD_NSRPC2COMMUNICATION_VEHICLEINFO__UNSUBSCRIBEVEHICLEDATARESPONSE,c.getId(),c.getResult())
{
  *this=c;
}


const std::vector< NsAppLinkRPCV2::VehicleDataResult>* UnsubscribeVehicleDataResponse::get_dataResult(void)
{
  return dataResult;
}

bool UnsubscribeVehicleDataResponse::set_dataResult(const std::vector< NsAppLinkRPCV2::VehicleDataResult>& dataResult_)
{
  if(dataResult)  delete dataResult;
  dataResult=new std::vector< NsAppLinkRPCV2::VehicleDataResult>(dataResult_);
  return true;
}

void UnsubscribeVehicleDataResponse::reset_dataResult(void)
{
  if(dataResult)  delete dataResult;
  dataResult=0;
}

bool UnsubscribeVehicleDataResponse::checkIntegrity(void)
{
  return UnsubscribeVehicleDataResponseMarshaller::checkIntegrity(*this);
}
