#include "../include/JSONHandler/ALRPCObjects/V2/OnTBTClientState.h"
#include "OnTBTClientStateMarshaller.h"
#include "../include/JSONHandler/ALRPCObjects/V2/Marshaller.h"
#include "TBTStateMarshaller.h"

#define PROTOCOL_VERSION	2


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Dec 13 14:18:29 2012
  source stamp	Thu Dec 13 14:18:27 2012
  author	robok0der
*/

using namespace NsAppLinkRPCV2;

OnTBTClientState::~OnTBTClientState(void)
{
}


OnTBTClientState::OnTBTClientState(const OnTBTClientState& c) : NsAppLinkRPC::ALRPCMessage(c)
{
  *this=c;
}


bool OnTBTClientState::checkIntegrity(void)
{
  return OnTBTClientStateMarshaller::checkIntegrity(*this);
}


OnTBTClientState::OnTBTClientState(void) : NsAppLinkRPC::ALRPCMessage(PROTOCOL_VERSION)
{
}



bool OnTBTClientState::set_state(const TBTState& state_)
{
  if(!TBTStateMarshaller::checkIntegrityConst(state_))   return false;
  state=state_;
  return true;
}




const TBTState& OnTBTClientState::get_state(void) const 
{
  return state;
}

