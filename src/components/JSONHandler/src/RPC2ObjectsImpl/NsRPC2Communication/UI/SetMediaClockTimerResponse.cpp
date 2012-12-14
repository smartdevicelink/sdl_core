#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/SetMediaClockTimerResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects/Marshaller.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Fri Dec 14 06:14:25 2012
  source stamp	Fri Dec 14 06:14:23 2012
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
