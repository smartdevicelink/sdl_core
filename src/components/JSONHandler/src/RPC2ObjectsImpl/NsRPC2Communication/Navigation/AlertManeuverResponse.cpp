#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/Navigation/AlertManeuverResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects/Marshaller.h"

/*
  interface	NsRPC2Communication::Navigation
  version	2.0
  generated at	Fri Dec 14 06:14:25 2012
  source stamp	Fri Dec 14 06:14:23 2012
  author	robok0der
*/

using namespace NsRPC2Communication::Navigation;


AlertManeuverResponse& AlertManeuverResponse::operator =(const AlertManeuverResponse& c)
{
  return *this;
}


AlertManeuverResponse::~AlertManeuverResponse(void)
{
}


AlertManeuverResponse::AlertManeuverResponse(void) : 
  RPC2Response(Marshaller::METHOD_NSRPC2COMMUNICATION_NAVIGATION__ALERTMANEUVERRESPONSE)
{
}


AlertManeuverResponse::AlertManeuverResponse(const AlertManeuverResponse& c) : RPC2Response(Marshaller::METHOD_NSRPC2COMMUNICATION_NAVIGATION__ALERTMANEUVERRESPONSE,c.getId(),c.getResult())
{
  *this=c;
}


bool AlertManeuverResponse::checkIntegrity(void)
{
  return AlertManeuverResponseMarshaller::checkIntegrity(*this);
}
