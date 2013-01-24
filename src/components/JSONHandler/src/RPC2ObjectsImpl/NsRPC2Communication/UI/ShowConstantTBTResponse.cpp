#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/ShowConstantTBTResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects/Marshaller.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	robok0der
*/

using namespace NsRPC2Communication::UI;


ShowConstantTBTResponse& ShowConstantTBTResponse::operator =(const ShowConstantTBTResponse& c)
{
  return *this;
}


ShowConstantTBTResponse::~ShowConstantTBTResponse(void)
{
}


ShowConstantTBTResponse::ShowConstantTBTResponse(void) : 
  RPC2Response(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__SHOWCONSTANTTBTRESPONSE)
{
}


ShowConstantTBTResponse::ShowConstantTBTResponse(const ShowConstantTBTResponse& c) : RPC2Response(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__SHOWCONSTANTTBTRESPONSE,c.getId(),c.getResult())
{
  *this=c;
}


bool ShowConstantTBTResponse::checkIntegrity(void)
{
  return ShowConstantTBTResponseMarshaller::checkIntegrity(*this);
}
