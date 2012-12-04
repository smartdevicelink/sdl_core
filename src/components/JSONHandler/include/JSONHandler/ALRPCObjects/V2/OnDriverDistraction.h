#ifndef NSAPPLINKRPCV2_ONDRIVERDISTRACTION_INCLUDE
#define NSAPPLINKRPCV2_ONDRIVERDISTRACTION_INCLUDE


#include "DriverDistractionState.h"
#include "JSONHandler/ALRPC2Message.hh"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Tue Dec  4 14:30:13 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

namespace NsAppLinkRPCV2
{

///  Provides driver distraction state to mobile applications

  class OnDriverDistraction : public ALRPC2Message
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
