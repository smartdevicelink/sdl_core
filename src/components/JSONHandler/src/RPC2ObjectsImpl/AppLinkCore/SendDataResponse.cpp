#include "../include/JSONHandler/RPC2Objects/AppLinkCore/SendDataResponse.h"
#include "../include/JSONHandler/RPC2Objects/AppLinkCore/Marshaller.h"

/*
  interface	RPC2Communication::AppLinkCore
  version	1.2
  generated at	Wed Nov  7 11:26:14 2012
  source stamp	Wed Nov  7 09:29:07 2012
  author	robok0der
*/

using namespace RPC2Communication;
using namespace AppLinkRPC;
using namespace RPC2Communication::AppLinkCore;


SendDataResponse& SendDataResponse::operator =(const SendDataResponse& c)
{
  return *this;
}


SendDataResponse::~SendDataResponse(void)
{
}


SendDataResponse::SendDataResponse(void) :
  RPC2Response(Marshaller::METHOD_SENDDATARESPONSE)
{
}


SendDataResponse::SendDataResponse(const SendDataResponse& c) : RPC2Response(Marshaller::METHOD_SENDDATARESPONSE,c.getId(),c.getResult())
{
  *this=c;
}


bool SendDataResponse::checkIntegrity(void)
{
  return SendDataResponseMarshaller::checkIntegrity(*this);
}

