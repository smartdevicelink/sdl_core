#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/SetMediaClockTimerResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects/Marshaller.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	robok0der
*/

using namespace NsRPC2Communication::UI;


SetMediaClockTimerResponse& SetMediaClockTimerResponse::operator =(const SetMediaClockTimerResponse& c)
{
  return *this;
}


SetMediaClockTimerResponse::~SetMediaClockTimerResponse(void)
{
}


SetMediaClockTimerResponse::SetMediaClockTimerResponse(void) : 
  RPC2Response(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__SETMEDIACLOCKTIMERRESPONSE)
{
}


SetMediaClockTimerResponse::SetMediaClockTimerResponse(const SetMediaClockTimerResponse& c) : RPC2Response(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__SETMEDIACLOCKTIMERRESPONSE,c.getId(),c.getResult())
{
  *this=c;
}


bool SetMediaClockTimerResponse::checkIntegrity(void)
{
  return SetMediaClockTimerResponseMarshaller::checkIntegrity(*this);
}
