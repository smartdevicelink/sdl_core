#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/OnTBTClientState.h"
#include "../src/../include/JSONHandler/RPC2Objects/Marshaller.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	robok0der
*/

using namespace NsRPC2Communication::UI;


OnTBTClientState& OnTBTClientState::operator =(const OnTBTClientState& c)
{
  state=c.state;
  return *this;
}


OnTBTClientState::~OnTBTClientState(void)
{
}


OnTBTClientState::OnTBTClientState(void) : 
  RPC2Notification(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__ONTBTCLIENTSTATE)
{
}


OnTBTClientState::OnTBTClientState(const OnTBTClientState& c) : RPC2Notification(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__ONTBTCLIENTSTATE)
{
  *this=c;
}


const NsAppLinkRPCV2::TBTState& OnTBTClientState::get_state(void)
{
  return state;
}

bool OnTBTClientState::set_state(const NsAppLinkRPCV2::TBTState& state_)
{
  state=state_;
  return true;
}

bool OnTBTClientState::checkIntegrity(void)
{
  return OnTBTClientStateMarshaller::checkIntegrity(*this);
}
