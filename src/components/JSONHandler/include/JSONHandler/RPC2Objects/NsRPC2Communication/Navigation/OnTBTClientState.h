#ifndef NSRPC2COMMUNICATION_NAVIGATION_ONTBTCLIENTSTATE_INCLUDE
#define NSRPC2COMMUNICATION_NAVIGATION_ONTBTCLIENTSTATE_INCLUDE

#include "JSONHandler/RPC2Notification.h"

#include "../include/JSONHandler/ALRPCObjects/V1/TBTState.h"

/*
  interface	NsRPC2Communication::Navigation
  version	2.0
  generated at	Fri Dec 14 06:14:25 2012
  source stamp	Fri Dec 14 06:14:23 2012
  author	robok0der
*/

namespace NsRPC2Communication
{
  namespace Navigation
  {

    class OnTBTClientState : public ::NsRPC2Communication::RPC2Notification
    {
    public:
    
      OnTBTClientState(const OnTBTClientState& c);
      OnTBTClientState(void);
    
      OnTBTClientState& operator =(const OnTBTClientState&);
    
      virtual ~OnTBTClientState(void);
    
      bool checkIntegrity(void);
    
// getters
      const NsAppLinkRPC::TBTState& get_state(void);


// setters
      bool set_state(const NsAppLinkRPC::TBTState& state);


    private:

      friend class OnTBTClientStateMarshaller;

      NsAppLinkRPC::TBTState state;

    };
  }
}

#endif
