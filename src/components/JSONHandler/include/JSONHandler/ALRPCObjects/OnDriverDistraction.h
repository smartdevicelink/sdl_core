#ifndef ONDRIVERDISTRACTION_INCLUDE
#define ONDRIVERDISTRACTION_INCLUDE


#include "DriverDistractionState.h"
#include "JSONHandler/ALRPCNotification.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Oct 25 06:32:04 2012
  source stamp	Thu Oct 25 06:28:28 2012
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
