#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/AppLinkCore/ActivateAppResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects//Marshaller.h"

/*
  interface	NsRPC2Communication::AppLinkCore
  version	1.2
  generated at	Tue Dec  4 15:06:30 2012
  source stamp	Tue Dec  4 14:21:32 2012
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
