#include "../../include/JSONHandler/ALRPCObjects/OnTBTClientState.h"
#include "OnTBTClientStateMarshaller.h"
#include "../../include/JSONHandler/ALRPCObjects/Marshaller.h"
#include "TBTStateMarshaller.h"

#define PROTOCOL_VERSION	1


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/


OnTBTClientState::~OnTBTClientState(void)
{
}


OnTBTClientState::OnTBTClientState(const OnTBTClientState& c)
{
  *this=c;
}


bool OnTBTClientState::checkIntegrity(void)
{
  return OnTBTClientStateMarshaller::checkIntegrity(*this);
}


OnTBTClientState::OnTBTClientState(void) : ALRPCNotification(PROTOCOL_VERSION,Marshaller::METHOD_ONTBTCLIENTSTATE)
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

