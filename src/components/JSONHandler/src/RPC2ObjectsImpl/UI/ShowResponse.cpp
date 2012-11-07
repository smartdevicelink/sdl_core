#include "../include/JSONHandler/RPC2Objects/UI/ShowResponse.h"
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


ShowResponse& ShowResponse::operator =(const ShowResponse& c)
{
  return *this;
}


ShowResponse::~ShowResponse(void)
{
}


ShowResponse::ShowResponse(void) : 
  RPC2Response(Marshaller::METHOD_SHOWRESPONSE)
{
}


ShowResponse::ShowResponse(const ShowResponse& c) : RPC2Response(Marshaller::METHOD_SHOWRESPONSE,c.getId(),c.getResult())
{
  *this=c;
}


bool ShowResponse::checkIntegrity(void)
{
  return ShowResponseMarshaller::checkIntegrity(*this);
}
