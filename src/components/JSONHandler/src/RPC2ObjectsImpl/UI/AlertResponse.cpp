#include "../include/JSONHandler/RPC2Objects/UI/AlertResponse.h"
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


AlertResponse& AlertResponse::operator =(const AlertResponse& c)
{
  return *this;
}


AlertResponse::~AlertResponse(void)
{
}


AlertResponse::AlertResponse(void) : 
  RPC2Response(Marshaller::METHOD_ALERTRESPONSE)
{
}


AlertResponse::AlertResponse(const AlertResponse& c) : RPC2Response(Marshaller::METHOD_ALERTRESPONSE,c.getId(),c.getResult())
{
  *this=c;
}


bool AlertResponse::checkIntegrity(void)
{
  return AlertResponseMarshaller::checkIntegrity(*this);
}
