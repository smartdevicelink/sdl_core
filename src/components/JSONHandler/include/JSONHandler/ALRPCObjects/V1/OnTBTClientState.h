#ifndef NSAPPLINKRPCV1_ONTBTCLIENTSTATE_INCLUDE
#define NSAPPLINKRPCV1_ONTBTCLIENTSTATE_INCLUDE


#include "TBTState.h"
#include "JSONHandler/ALRPCNotification.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Tue Dec  4 14:55:10 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

namespace NsAppLinkRPCV1
{

///  Provides applications with notifications specific to the current TBT client status on the module

  class OnTBTClientState : public ALRPCNotification
  {
  public:
  
    OnTBTClientState(const OnTBTClientState& c);
    OnTBTClientState(void);
    
    virtual ~OnTBTClientState(void);
  
    bool checkIntegrity(void);

    const TBTState& get_state(void) const;

    bool set_state(const TBTState& state_);

  private:
  
    friend class OnTBTClientStateMarshaller;


///  Current State of TBT client
      TBTState state;
  };

}

#endif
