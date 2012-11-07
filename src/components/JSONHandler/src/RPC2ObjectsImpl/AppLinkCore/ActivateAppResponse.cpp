#include "../include/JSONHandler/RPC2Objects/AppLinkCore/ActivateAppResponse.h"
#include "../include/JSONHandler/RPC2Objects/AppLinkCore/Marshaller.h"

/*
  interface	RPC2Communication::AppLinkCore
  version	1.2
  generated at	Wed Nov  7 11:25:43 2012
  source stamp	Wed Nov  7 09:35:35 2012
  author	robok0der
*/

using namespace RPC2Communication;
using namespace AppLinkRPC;
using namespace RPC2Communication::AppLinkCore;


ActivateAppResponse& ActivateAppResponse::operator =(const ActivateAppResponse& c)
{
  return *this;
}


ActivateAppResponse::~ActivateAppResponse(void)
{
}


ActivateAppResponse::ActivateAppResponse(void) : 
  RPC2Response(Marshaller::METHOD_ACTIVATEAPPRESPONSE)
{
}


ActivateAppResponse::ActivateAppResponse(const ActivateAppResponse& c) : RPC2Response(Marshaller::METHOD_ACTIVATEAPPRESPONSE,c.getId(),c.getResult())
{
  *this=c;
}


bool ActivateAppResponse::checkIntegrity(void)
{
  return ActivateAppResponseMarshaller::checkIntegrity(*this);
}
