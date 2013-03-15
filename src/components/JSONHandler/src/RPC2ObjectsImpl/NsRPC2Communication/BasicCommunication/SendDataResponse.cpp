//
// Copyright (c) 2013 Ford Motor Company
//

#include "../include/JSONHandler/RPC2Objects/NsRPC2Communication/BasicCommunication/SendDataResponse.h"
#include "../include/JSONHandler/RPC2Objects//Marshaller.h"

/*
  interface	NsRPC2Communication::BasicCommunication
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	RC
*/

using namespace NsRPC2Communication::BasicCommunication;


SendDataResponse& SendDataResponse::operator =(const SendDataResponse& c)
{
  return *this;
}


SendDataResponse::~SendDataResponse(void)
{
}


SendDataResponse::SendDataResponse(void) : 
  RPC2Response(Marshaller::METHOD_NSRPC2COMMUNICATION_BASICCOMMUNICATION__SENDDATARESPONSE)
{
}


SendDataResponse::SendDataResponse(const SendDataResponse& c) : RPC2Response(Marshaller::METHOD_NSRPC2COMMUNICATION_BASICCOMMUNICATION__SENDDATARESPONSE,c.getId(),c.getResult())
{
  *this=c;
}


bool SendDataResponse::checkIntegrity(void)
{
  return SendDataResponseMarshaller::checkIntegrity(*this);
}
