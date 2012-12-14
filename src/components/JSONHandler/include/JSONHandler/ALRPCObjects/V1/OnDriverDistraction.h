#ifndef NSAPPLINKRPC_ONDRIVERDISTRACTION_INCLUDE
#define NSAPPLINKRPC_ONDRIVERDISTRACTION_INCLUDE


#include "DriverDistractionState.h"
#include "JSONHandler/ALRPCNotification.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Fri Dec 14 08:11:37 2012
  source stamp	Fri Dec 14 08:11:34 2012
  author	robok0der
*/

namespace NsAppLinkRPC
{

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


///  Current State of Driver Distraction
      DriverDistractionState state;
  };

}

#endif
