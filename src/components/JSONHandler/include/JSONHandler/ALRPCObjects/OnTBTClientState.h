#ifndef NSAPPLINKRPC_ONTBTCLIENTSTATE_INCLUDE
#define NSAPPLINKRPC_ONTBTCLIENTSTATE_INCLUDE


#include "TBTState.h"
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
