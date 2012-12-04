#include "../include/JSONHandler/ALRPCObjects/V2/OnDriverDistraction.h"
#include "OnDriverDistractionMarshaller.h"
#include "../include/JSONHandler/ALRPCObjects/V2/Marshaller.h"
#include "DriverDistractionStateMarshaller.h"

#define PROTOCOL_VERSION	2


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Tue Dec  4 14:30:13 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

using namespace NsAppLinkRPCV2;

OnDriverDistraction::~OnDriverDistraction(void)
{
}


OnDriverDistraction::OnDriverDistraction(const OnDriverDistraction& c) : ALRPC2Message(c)
{
  *this=c;
}


bool OnDriverDistraction::checkIntegrity(void)
{
  return OnDriverDistractionMarshaller::checkIntegrity(*this);
}


OnDriverDistraction::OnDriverDistraction(void) : ALRPC2Message(PROTOCOL_VERSION)
{
}



bool OnDriverDistraction::set_state(const DriverDistractionState& state_)
{
  if(!DriverDistractionStateMarshaller::checkIntegrityConst(state_))   return false;
  state=state_;
  return true;
}




const DriverDistractionState& OnDriverDistraction::get_state(void) const 
{
  return state;
}

