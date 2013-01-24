#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/AppLinkCore/OnAppDeactivatedResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects//Marshaller.h"

/*
  interface	NsRPC2Communication::AppLinkCore
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	robok0der
*/

using namespace NsRPC2Communication::AppLinkCore;


OnAppDeactivatedResponse& OnAppDeactivatedResponse::operator =(const OnAppDeactivatedResponse& c)
{
  return *this;
}


OnAppDeactivatedResponse::~OnAppDeactivatedResponse(void)
{
}


OnAppDeactivatedResponse::OnAppDeactivatedResponse(void) : 
  RPC2Response(Marshaller::METHOD_NSRPC2COMMUNICATION_APPLINKCORE__ONAPPDEACTIVATEDRESPONSE)
{
}


OnAppDeactivatedResponse::OnAppDeactivatedResponse(const OnAppDeactivatedResponse& c) : RPC2Response(Marshaller::METHOD_NSRPC2COMMUNICATION_APPLINKCORE__ONAPPDEACTIVATEDRESPONSE,c.getId(),c.getResult())
{
  *this=c;
}


bool OnAppDeactivatedResponse::checkIntegrity(void)
{
  return OnAppDeactivatedResponseMarshaller::checkIntegrity(*this);
}
