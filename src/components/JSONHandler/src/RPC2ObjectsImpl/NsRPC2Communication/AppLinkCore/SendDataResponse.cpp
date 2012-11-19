#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/AppLinkCore/SendDataResponse.h"
#include "../../../../../Marshaller.h"

/*
  interface	NsRPC2Communication::AppLinkCore
  version	1.2
  generated at	Mon Nov 19 07:25:04 2012
  source stamp	Mon Nov 19 07:04:40 2012
  author	robok0der
*/

using namespace NsRPC2Communication::AppLinkCore;


SendDataResponse& SendDataResponse::operator =(const SendDataResponse& c)
{
  return *this;
}


SendDataResponse::~SendDataResponse(void)
{
}


SendDataResponse::SendDataResponse(void) : 
  RPC2Response(Marshaller::METHOD_NSRPC2COMMUNICATION_APPLINKCORE__SENDDATARESPONSE)
{
}


SendDataResponse::SendDataResponse(const SendDataResponse& c) : RPC2Response(Marshaller::METHOD_NSRPC2COMMUNICATION_APPLINKCORE__SENDDATARESPONSE,c.getId(),c.getResult())
{
  *this=c;
}


bool SendDataResponse::checkIntegrity(void)
{
  return SendDataResponseMarshaller::checkIntegrity(*this);
}
