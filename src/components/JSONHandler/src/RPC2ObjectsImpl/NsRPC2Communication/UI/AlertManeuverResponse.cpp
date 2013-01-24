#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/AlertManeuverResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects/Marshaller.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	robok0der
*/

using namespace NsRPC2Communication::UI;


AlertManeuverResponse& AlertManeuverResponse::operator =(const AlertManeuverResponse& c)
{
  return *this;
}


AlertManeuverResponse::~AlertManeuverResponse(void)
{
}


AlertManeuverResponse::AlertManeuverResponse(void) : 
  RPC2Response(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__ALERTMANEUVERRESPONSE)
{
}


AlertManeuverResponse::AlertManeuverResponse(const AlertManeuverResponse& c) : RPC2Response(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__ALERTMANEUVERRESPONSE,c.getId(),c.getResult())
{
  *this=c;
}


bool AlertManeuverResponse::checkIntegrity(void)
{
  return AlertManeuverResponseMarshaller::checkIntegrity(*this);
}
