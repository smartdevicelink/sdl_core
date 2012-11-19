#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/ShowResponse.h"
#include "../../../../../Marshaller.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Mon Nov 19 10:37:39 2012
  source stamp	Mon Nov 19 10:17:20 2012
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
