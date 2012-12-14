#ifndef NSRPC2COMMUNICATION_BUTTONS_ONBUTTONEVENT_INCLUDE
#define NSRPC2COMMUNICATION_BUTTONS_ONBUTTONEVENT_INCLUDE

#include "JSONHandler/RPC2Notification.h"

#include "../include/JSONHandler/ALRPCObjects/V1/ButtonName.h"
#include "../include/JSONHandler/ALRPCObjects/V1/ButtonEventMode.h"

/*
  interface	NsRPC2Communication::Buttons
  version	1.2
  generated at	Fri Dec 14 06:14:25 2012
  source stamp	Fri Dec 14 06:14:23 2012
  author	robok0der
*/

namespace NsRPC2Communication
{
  namespace Buttons
  {

    class OnButtonEvent : public ::NsRPC2Communication::RPC2Notification
    {
    public:
    
      OnButtonEvent(const OnButtonEvent& c);
      OnButtonEvent(void);
    
      OnButtonEvent& operator =(const OnButtonEvent&);
    
      virtual ~OnButtonEvent(void);
    
      bool checkIntegrity(void);
    
// getters
      const NsAppLinkRPC::ButtonName& get_name(void);

      const NsAppLinkRPC::ButtonEventMode& get_mode(void);

      const unsigned int* get_customButtonID(void);

// setters
      bool set_name(const NsAppLinkRPC::ButtonName& name);

      bool set_mode(const NsAppLinkRPC::ButtonEventMode& mode);

/// customButtonID <= 65536
      bool set_customButtonID(const unsigned int& customButtonID);

      void reset_customButtonID(void);


    private:

      friend class OnButtonEventMarshaller;

      NsAppLinkRPC::ButtonName name;
      NsAppLinkRPC::ButtonEventMode mode;
      unsigned int* customButtonID;

    };
  }
}

#endif
