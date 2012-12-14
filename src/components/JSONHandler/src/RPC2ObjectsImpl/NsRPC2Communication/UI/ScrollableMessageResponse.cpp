#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/ScrollableMessageResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects/Marshaller.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Fri Dec 14 06:14:25 2012
  source stamp	Fri Dec 14 06:14:23 2012
  author	robok0der
*/

using namespace NsRPC2Communication::UI;


ScrollableMessageResponse& ScrollableMessageResponse::operator =(const ScrollableMessageResponse& c)
{
  return *this;
}


ScrollableMessageResponse::~ScrollableMessageResponse(void)
{
}


ScrollableMessageResponse::ScrollableMessageResponse(void) : 
  RPC2Response(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__SCROLLABLEMESSAGERESPONSE)
{
}


ScrollableMessageResponse::ScrollableMessageResponse(const ScrollableMessageResponse& c) : RPC2Response(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__SCROLLABLEMESSAGERESPONSE,c.getId(),c.getResult())
{
  *this=c;
}


bool ScrollableMessageResponse::checkIntegrity(void)
{
  return ScrollableMessageResponseMarshaller::checkIntegrity(*this);
}
