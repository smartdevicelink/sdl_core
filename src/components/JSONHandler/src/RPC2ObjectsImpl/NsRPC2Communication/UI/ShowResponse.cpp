#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/ShowResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects/Marshaller.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	robok0der
*/

using namespace NsRPC2Communication::UI;


ShowResponse& ShowResponse::operator =(const ShowResponse& c)
{
  return *this;
}


ShowResponse::~ShowResponse(void)
{
}


ShowResponse::ShowResponse(void) : 
  RPC2Response(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__SHOWRESPONSE)
{
}


ShowResponse::ShowResponse(const ShowResponse& c) : RPC2Response(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__SHOWRESPONSE,c.getId(),c.getResult())
{
  *this=c;
}


bool ShowResponse::checkIntegrity(void)
{
  return ShowResponseMarshaller::checkIntegrity(*this);
}
