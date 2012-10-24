#include "../../include/JSONHandler/ALRPCObjects/OnDriverDistraction.h"
#include "OnDriverDistractionMarshaller.h"
#include "../../include/JSONHandler/ALRPCObjects/Marshaller.h"
#include "DriverDistractionStateMarshaller.h"

#define PROTOCOL_VERSION	1


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/


OnDriverDistraction::~OnDriverDistraction(void)
{
}


OnDriverDistraction::OnDriverDistraction(const OnDriverDistraction& c)
{
  *this=c;
}


bool OnDriverDistraction::checkIntegrity(void)
{
  return OnDriverDistractionMarshaller::checkIntegrity(*this);
}


OnDriverDistraction::OnDriverDistraction(void) : ALRPCNotification(PROTOCOL_VERSION,Marshaller::METHOD_ONDRIVERDISTRACTION)
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

