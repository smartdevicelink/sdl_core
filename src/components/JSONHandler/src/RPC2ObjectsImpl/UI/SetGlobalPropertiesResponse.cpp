#include "../include/JSONHandler/RPC2Objects/UI/SetGlobalPropertiesResponse.h"
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


SetGlobalPropertiesResponse& SetGlobalPropertiesResponse::operator =(const SetGlobalPropertiesResponse& c)
{
  return *this;
}


SetGlobalPropertiesResponse::~SetGlobalPropertiesResponse(void)
{
}


SetGlobalPropertiesResponse::SetGlobalPropertiesResponse(void) : 
  RPC2Response(Marshaller::METHOD_SETGLOBALPROPERTIESRESPONSE)
{
}


SetGlobalPropertiesResponse::SetGlobalPropertiesResponse(const SetGlobalPropertiesResponse& c) : RPC2Response(Marshaller::METHOD_SETGLOBALPROPERTIESRESPONSE,c.getId(),c.getResult())
{
  *this=c;
}


bool SetGlobalPropertiesResponse::checkIntegrity(void)
{
  return SetGlobalPropertiesResponseMarshaller::checkIntegrity(*this);
}
