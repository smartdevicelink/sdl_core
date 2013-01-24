#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/VehicleInfo/GetDTCsResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects/Marshaller.h"

/*
  interface	NsRPC2Communication::VehicleInfo
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	robok0der
*/

using namespace NsRPC2Communication::VehicleInfo;


GetDTCsResponse& GetDTCsResponse::operator =(const GetDTCsResponse& c)
{
  if(dtcList)  delete dtcList;
  dtcList= c.dtcList ? new std::vector<NsAppLinkRPCV2::DTC>(c.dtcList[0]) : 0;
  return *this;
}


GetDTCsResponse::~GetDTCsResponse(void)
{
  if(dtcList)  delete dtcList;
}


GetDTCsResponse::GetDTCsResponse(void) : 
  RPC2Response(Marshaller::METHOD_NSRPC2COMMUNICATION_VEHICLEINFO__GETDTCSRESPONSE),
  dtcList(0)
{
}


GetDTCsResponse::GetDTCsResponse(const GetDTCsResponse& c) : RPC2Response(Marshaller::METHOD_NSRPC2COMMUNICATION_VEHICLEINFO__GETDTCSRESPONSE,c.getId(),c.getResult())
{
  *this=c;
}


const std::vector< NsAppLinkRPCV2::DTC>* GetDTCsResponse::get_dtcList(void)
{
  return dtcList;
}

bool GetDTCsResponse::set_dtcList(const std::vector< NsAppLinkRPCV2::DTC>& dtcList_)
{
  if(dtcList)  delete dtcList;
  dtcList=new std::vector< NsAppLinkRPCV2::DTC>(dtcList_);
  return true;
}

void GetDTCsResponse::reset_dtcList(void)
{
  if(dtcList)  delete dtcList;
  dtcList=0;
}

bool GetDTCsResponse::checkIntegrity(void)
{
  return GetDTCsResponseMarshaller::checkIntegrity(*this);
}
