#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/AlertResponse.h"
#include "../../../../../Marshaller.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Mon Nov 19 08:20:05 2012
  source stamp	Mon Nov 19 07:04:40 2012
  author	robok0der
*/

using namespace NsRPC2Communication::UI;


AlertResponse& AlertResponse::operator =(const AlertResponse& c)
{
  return *this;
}


AlertResponse::~AlertResponse(void)
{
}


AlertResponse::AlertResponse(void) : 
  RPC2Response(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__ALERTRESPONSE)
{
}


AlertResponse::AlertResponse(const AlertResponse& c) : RPC2Response(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__ALERTRESPONSE,c.getId(),c.getResult())
{
  *this=c;
}


bool AlertResponse::checkIntegrity(void)
{
  return AlertResponseMarshaller::checkIntegrity(*this);
}
