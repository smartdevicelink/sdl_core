#ifndef NSAPPLINKRPC_ONTBTCLIENTSTATE_V2_INCLUDE
#define NSAPPLINKRPC_ONTBTCLIENTSTATE_V2_INCLUDE


#include "TBTState_v2.h"
#include "JSONHandler/ALRPCNotification.h"


/*
  interface	Ford Sync RAPI
  version	1.2 / 2.0O
  date		2011-05-17 / 2012-11-02
  generated at	Thu Nov 29 14:49:08 2012
  source stamp	Thu Nov 29 06:50:10 2012
  author	robok0der
*/

namespace NsAppLinkRPC
{

///  Provides applications with notifications specific to the current TBT client status on the module

  class OnTBTClientState_v2 : public ALRPCNotification
  {
  public:
  
    OnTBTClientState_v2(const OnTBTClientState_v2& c);
    OnTBTClientState_v2(void);
    
    virtual ~OnTBTClientState_v2(void);
  
    bool checkIntegrity(void);

    const TBTState_v2& get_state(void) const;

    bool set_state(const TBTState_v2& state_);

  private:
  
    friend class OnTBTClientState_v2Marshaller;


///  Current State of TBT client
      TBTState_v2 state;
  };

}

#endif
