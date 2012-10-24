#ifndef ONDRIVERDISTRACTION_INCLUDE
#define ONDRIVERDISTRACTION_INCLUDE


#include "DriverDistractionState.h"
#include "../../../JSONHandler/ALRPCNotification.hh"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/


///  Provides driver distraction state to mobile applications

class OnDriverDistraction : public ALRPCNotification
{
public:

  OnDriverDistraction(const OnDriverDistraction& c);
  OnDriverDistraction(void);
  
  virtual ~OnDriverDistraction(void);

  bool checkIntegrity(void);

  const DriverDistractionState& get_state(void) const;

  bool set_state(const DriverDistractionState& state_);

private:

  friend class OnDriverDistractionMarshaller;

  DriverDistractionState state;
};

#endif
