#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/AppLinkCore/DeactivateAppResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects//Marshaller.h"

/*
  interface	NsRPC2Communication::AppLinkCore
  version	1.2
  generated at	Thu Nov 29 14:32:09 2012
  source stamp	Thu Nov 29 14:32:05 2012
  author	robok0der
*/

using namespace NsRPC2Communication::AppLinkCore;


DeactivateAppResponse& DeactivateAppResponse::operator =(const DeactivateAppResponse& c)
{
  return *this;
}


DeactivateAppResponse::~DeactivateAppResponse(void)
{
}


DeactivateAppResponse::DeactivateAppResponse(void) : 
  RPC2Response(Marshaller::METHOD_NSRPC2COMMUNICATION_APPLINKCORE__DEACTIVATEAPPRESPONSE)
{
}


DeactivateAppResponse::DeactivateAppResponse(const DeactivateAppResponse& c) : RPC2Response(Marshaller::METHOD_NSRPC2COMMUNICATION_APPLINKCORE__DEACTIVATEAPPRESPONSE,c.getId(),c.getResult())
{
  *this=c;
}


bool DeactivateAppResponse::checkIntegrity(void)
{
  return DeactivateAppResponseMarshaller::checkIntegrity(*this);
}
