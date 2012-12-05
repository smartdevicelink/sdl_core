#ifndef NSRPC2COMMUNICATION_UI_ONDRIVERDISTRACTION_INCLUDE
#define NSRPC2COMMUNICATION_UI_ONDRIVERDISTRACTION_INCLUDE

#include "JSONHandler/RPC2Notification.h"

#include "../include/JSONHandler/ALRPCObjects/V1/DriverDistractionState.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Tue Dec  4 16:38:13 2012
  source stamp	Tue Dec  4 16:37:04 2012
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
      const NsAppLinkRPC::DriverDistractionState& get_state(void);

      int get_appId(void);


// setters
      bool set_state(const NsAppLinkRPC::DriverDistractionState& state);

      bool set_appId(int appId);


    private:

      friend class OnDriverDistractionMarshaller;

      NsAppLinkRPC::DriverDistractionState state;
      int appId;

    };
  }
}

#endif
