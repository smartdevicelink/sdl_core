//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSSMARTDEVICELINKKRPCV2_ONDRIVERDISTRACTION_INCLUDE
#define NSSMARTDEVICELINKKRPCV2_ONDRIVERDISTRACTION_INCLUDE


#include "DriverDistractionState.h"
#include "JSONHandler/SDLRPCMessage.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	RC
*/

namespace NsSmartDeviceLinkRPCV2
{

///  Provides driver distraction state to mobile applications

  class OnDriverDistraction : public NsSmartDeviceLinkRPC::SDLRPCMessage
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
