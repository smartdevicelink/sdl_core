#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/OnDriverDistraction.h"
#include "../src/../include/JSONHandler/RPC2Objects/Marshaller.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Fri Dec 14 06:14:25 2012
  source stamp	Fri Dec 14 06:14:23 2012
  author	robok0der
*/

using namespace NsRPC2Communication::UI;


OnDriverDistraction& OnDriverDistraction::operator =(const OnDriverDistraction& c)
{
  state=c.state;
  return *this;
}


OnDriverDistraction::~OnDriverDistraction(void)
{
}


OnDriverDistraction::OnDriverDistraction(void) : 
  RPC2Notification(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__ONDRIVERDISTRACTION)
{
}


OnDriverDistraction::OnDriverDistraction(const OnDriverDistraction& c) : RPC2Notification(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__ONDRIVERDISTRACTION)
{
  *this=c;
}


const NsAppLinkRPC::DriverDistractionState& OnDriverDistraction::get_state(void)
{
  return state;
}

bool OnDriverDistraction::set_state(const NsAppLinkRPC::DriverDistractionState& state_)
{
  state=state_;
  return true;
}

bool OnDriverDistraction::checkIntegrity(void)
{
  return OnDriverDistractionMarshaller::checkIntegrity(*this);
}
