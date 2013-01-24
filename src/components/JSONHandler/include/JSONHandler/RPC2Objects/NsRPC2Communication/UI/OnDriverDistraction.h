#ifndef NSRPC2COMMUNICATION_UI_ONDRIVERDISTRACTION_INCLUDE
#define NSRPC2COMMUNICATION_UI_ONDRIVERDISTRACTION_INCLUDE

#include "JSONHandler/RPC2Notification.h"

#include "../include/JSONHandler/ALRPCObjects/V2/DriverDistractionState.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	robok0der
*/

namespace NsRPC2Communication
{
  namespace UI
  {

    class OnDriverDistraction : public ::NsRPC2Communication::RPC2Notification
    {
    public:
    
      OnDriverDistraction(const OnDriverDistraction& c);
      OnDriverDistraction(void);
    
      OnDriverDistraction& operator =(const OnDriverDistraction&);
    
      virtual ~OnDriverDistraction(void);
    
      bool checkIntegrity(void);
    
// getters
      const NsAppLinkRPCV2::DriverDistractionState& get_state(void);


// setters
      bool set_state(const NsAppLinkRPCV2::DriverDistractionState& state);


    private:

      friend class OnDriverDistractionMarshaller;

      NsAppLinkRPCV2::DriverDistractionState state;

    };
  }
}

#endif
