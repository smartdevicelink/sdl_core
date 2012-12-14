#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/Navigation/ShowConstantTBTResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects/Marshaller.h"

/*
  interface	NsRPC2Communication::Navigation
  version	2.0
  generated at	Fri Dec 14 06:14:25 2012
  source stamp	Fri Dec 14 06:14:23 2012
  author	robok0der
*/

using namespace NsRPC2Communication::Navigation;


ShowConstantTBTResponse& ShowConstantTBTResponse::operator =(const ShowConstantTBTResponse& c)
{
  return *this;
}


ShowConstantTBTResponse::~ShowConstantTBTResponse(void)
{
}


ShowConstantTBTResponse::ShowConstantTBTResponse(void) : 
  RPC2Response(Marshaller::METHOD_NSRPC2COMMUNICATION_NAVIGATION__SHOWCONSTANTTBTRESPONSE)
{
}


ShowConstantTBTResponse::ShowConstantTBTResponse(const ShowConstantTBTResponse& c) : RPC2Response(Marshaller::METHOD_NSRPC2COMMUNICATION_NAVIGATION__SHOWCONSTANTTBTRESPONSE,c.getId(),c.getResult())
{
  *this=c;
}


bool ShowConstantTBTResponse::checkIntegrity(void)
{
  return ShowConstantTBTResponseMarshaller::checkIntegrity(*this);
}
