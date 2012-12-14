#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/Navigation/OnTBTClientState.h"
#include "../src/../include/JSONHandler/RPC2Objects/Marshaller.h"

/*
  interface	NsRPC2Communication::Navigation
  version	2.0
  generated at	Fri Dec 14 06:14:25 2012
  source stamp	Fri Dec 14 06:14:23 2012
  author	robok0der
*/

using namespace NsRPC2Communication::Navigation;


OnTBTClientState& OnTBTClientState::operator =(const OnTBTClientState& c)
{
  state=c.state;
  return *this;
}


OnTBTClientState::~OnTBTClientState(void)
{
}


OnTBTClientState::OnTBTClientState(void) : 
  RPC2Notification(Marshaller::METHOD_NSRPC2COMMUNICATION_NAVIGATION__ONTBTCLIENTSTATE)
{
}


OnTBTClientState::OnTBTClientState(const OnTBTClientState& c) : RPC2Notification(Marshaller::METHOD_NSRPC2COMMUNICATION_NAVIGATION__ONTBTCLIENTSTATE)
{
  *this=c;
}


const NsAppLinkRPC::TBTState& OnTBTClientState::get_state(void)
{
  return state;
}

bool OnTBTClientState::set_state(const NsAppLinkRPC::TBTState& state_)
{
  state=state_;
  return true;
}

bool OnTBTClientState::checkIntegrity(void)
{
  return OnTBTClientStateMarshaller::checkIntegrity(*this);
}
