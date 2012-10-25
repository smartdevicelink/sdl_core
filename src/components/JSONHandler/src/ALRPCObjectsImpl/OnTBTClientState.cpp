#include "../../include/JSONHandler/ALRPCObjects/OnTBTClientState.h"
#include "OnTBTClientStateMarshaller.h"
#include "../../include/JSONHandler/ALRPCObjects/Marshaller.h"
#include "TBTStateMarshaller.h"

#define PROTOCOL_VERSION	1


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Oct 25 06:32:04 2012
  source stamp	Thu Oct 25 06:28:28 2012
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

