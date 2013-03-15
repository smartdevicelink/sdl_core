//
// Copyright (c) 2013 Ford Motor Company
//

#include "../include/JSONHandler/SDLRPCObjects/V1/OnTBTClientState.h"
#include "OnTBTClientStateMarshaller.h"
#include "../include/JSONHandler/SDLRPCObjects/V1/Marshaller.h"
#include "TBTStateMarshaller.h"

#define PROTOCOL_VERSION	1


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Jan 24 06:36:21 2013
  source stamp	Thu Jan 24 06:35:34 2013
  author	RC
*/

using namespace NsSmartDeviceLinkRPC;

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


OnTBTClientState::OnTBTClientState(void) : SDLRPCNotification(PROTOCOL_VERSION,Marshaller::METHOD_ONTBTCLIENTSTATE)
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

