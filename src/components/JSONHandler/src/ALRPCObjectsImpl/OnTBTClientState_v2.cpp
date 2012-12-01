#include "../include/JSONHandler/ALRPCObjects/OnTBTClientState_v2.h"
#include "OnTBTClientState_v2Marshaller.h"
#include "../include/JSONHandler/ALRPCObjects/Marshaller.h"
#include "TBTState_v2Marshaller.h"

#define PROTOCOL_VERSION	42


/*
  interface	Ford Sync RAPI
  version	1.2 / 2.0O
  date		2011-05-17 / 2012-11-02
  generated at	Thu Nov 29 14:49:08 2012
  source stamp	Thu Nov 29 06:50:10 2012
  author	robok0der
*/

using namespace NsAppLinkRPC;

OnTBTClientState_v2::~OnTBTClientState_v2(void)
{
}


OnTBTClientState_v2::OnTBTClientState_v2(const OnTBTClientState_v2& c)
{
  *this=c;
}


bool OnTBTClientState_v2::checkIntegrity(void)
{
  return OnTBTClientState_v2Marshaller::checkIntegrity(*this);
}


OnTBTClientState_v2::OnTBTClientState_v2(void) : ALRPCNotification(PROTOCOL_VERSION,Marshaller::METHOD_ONTBTCLIENTSTATE_V2)
{
}



bool OnTBTClientState_v2::set_state(const TBTState_v2& state_)
{
  if(!TBTState_v2Marshaller::checkIntegrityConst(state_))   return false;
  state=state_;
  return true;
}




const TBTState_v2& OnTBTClientState_v2::get_state(void) const 
{
  return state;
}

