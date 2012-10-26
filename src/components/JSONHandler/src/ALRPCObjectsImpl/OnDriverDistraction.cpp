#include "../../include/JSONHandler/ALRPCObjects/OnDriverDistraction.h"
#include "OnDriverDistractionMarshaller.h"
#include "../../include/JSONHandler/ALRPCObjects/Marshaller.h"
#include "DriverDistractionStateMarshaller.h"

#define PROTOCOL_VERSION	1


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Fri Oct 26 06:31:48 2012
  source stamp	Thu Oct 25 06:49:27 2012
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

