//
// Copyright (c) 2013 Ford Motor Company
//

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/VehicleInfo/ReadDIDResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects/Marshaller.h"

/*
  interface	NsRPC2Communication::VehicleInfo
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	RC
*/

using namespace NsRPC2Communication::VehicleInfo;


ReadDIDResponse& ReadDIDResponse::operator =(const ReadDIDResponse& c)
{
  if(dataResult)  delete dataResult;
  dataResult= c.dataResult ? new std::vector<NsSmartDeviceLinkRPCV2::VehicleDataResultCode>(c.dataResult[0]) : 0;
  if(data)  delete data;
  data= c.data ? new std::vector<std::string>(c.data[0]) : 0;
  return *this;
}


ReadDIDResponse::~ReadDIDResponse(void)
{
  if(dataResult)  delete dataResult;
  if(data)  delete data;
}


ReadDIDResponse::ReadDIDResponse(void) : 
  RPC2Response(Marshaller::METHOD_NSRPC2COMMUNICATION_VEHICLEINFO__READDIDRESPONSE),
  dataResult(0),
  data(0)
{
}


ReadDIDResponse::ReadDIDResponse(const ReadDIDResponse& c) : RPC2Response(Marshaller::METHOD_NSRPC2COMMUNICATION_VEHICLEINFO__READDIDRESPONSE,c.getId(),c.getResult())
{
  *this=c;
}


const std::vector< NsSmartDeviceLinkRPCV2::VehicleDataResultCode>* ReadDIDResponse::get_dataResult(void)
{
  return dataResult;
}

bool ReadDIDResponse::set_dataResult(const std::vector< NsSmartDeviceLinkRPCV2::VehicleDataResultCode>& dataResult_)
{
  if(dataResult)  delete dataResult;
  dataResult=new std::vector< NsSmartDeviceLinkRPCV2::VehicleDataResultCode>(dataResult_);
  return true;
}

void ReadDIDResponse::reset_dataResult(void)
{
  if(dataResult)  delete dataResult;
  dataResult=0;
}

const std::vector< std::string>* ReadDIDResponse::get_data(void)
{
  return data;
}

bool ReadDIDResponse::set_data(const std::vector< std::string>& data_)
{
  if(data)  delete data;
  data=new std::vector< std::string>(data_);
  return true;
}

void ReadDIDResponse::reset_data(void)
{
  if(data)  delete data;
  data=0;
}

bool ReadDIDResponse::checkIntegrity(void)
{
  return ReadDIDResponseMarshaller::checkIntegrity(*this);
}
