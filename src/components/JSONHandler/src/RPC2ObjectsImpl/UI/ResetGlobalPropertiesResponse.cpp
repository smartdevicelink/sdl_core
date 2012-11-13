#include "../include/JSONHandler/RPC2Objects/UI/ResetGlobalPropertiesResponse.h"
#include "../include/JSONHandler/RPC2Objects/UI/Marshaller.h"

/*
  interface	RPC2Communication::UI
  version	1.2
  generated at	Wed Nov  7 11:26:14 2012
  source stamp	Wed Nov  7 09:29:07 2012
  author	robok0der
*/

using namespace RPC2Communication;
using namespace AppLinkRPC;
using namespace RPC2Communication::UI;


ResetGlobalPropertiesResponse& ResetGlobalPropertiesResponse::operator =(const ResetGlobalPropertiesResponse& c)
{
  return *this;
}


ResetGlobalPropertiesResponse::~ResetGlobalPropertiesResponse(void)
{
}


ResetGlobalPropertiesResponse::ResetGlobalPropertiesResponse(void) : 
  RPC2Response(Marshaller::METHOD_RESETGLOBALPROPERTIESRESPONSE)
{
}


ResetGlobalPropertiesResponse::ResetGlobalPropertiesResponse(const ResetGlobalPropertiesResponse& c) : RPC2Response(Marshaller::METHOD_RESETGLOBALPROPERTIESRESPONSE,c.getId(),c.getResult())
{
  *this=c;
}


bool ResetGlobalPropertiesResponse::checkIntegrity(void)
{
  return ResetGlobalPropertiesResponseMarshaller::checkIntegrity(*this);
}
