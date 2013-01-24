#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/AppLinkCore/ActivateAppResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects//Marshaller.h"

/*
  interface	NsRPC2Communication::AppLinkCore
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	robok0der
*/

using namespace NsRPC2Communication::AppLinkCore;


ActivateAppResponse& ActivateAppResponse::operator =(const ActivateAppResponse& c)
{
  return *this;
}


ActivateAppResponse::~ActivateAppResponse(void)
{
}


ActivateAppResponse::ActivateAppResponse(void) : 
  RPC2Response(Marshaller::METHOD_NSRPC2COMMUNICATION_APPLINKCORE__ACTIVATEAPPRESPONSE)
{
}


ActivateAppResponse::ActivateAppResponse(const ActivateAppResponse& c) : RPC2Response(Marshaller::METHOD_NSRPC2COMMUNICATION_APPLINKCORE__ACTIVATEAPPRESPONSE,c.getId(),c.getResult())
{
  *this=c;
}


bool ActivateAppResponse::checkIntegrity(void)
{
  return ActivateAppResponseMarshaller::checkIntegrity(*this);
}
