#ifndef RPC2COMMUNICATION_BUTTONS_ONBUTTONEVENT_INCLUDE
#define RPC2COMMUNICATION_BUTTONS_ONBUTTONEVENT_INCLUDE

#include "JSONHandler/RPC2Notification.h"

#include "../include/JSONHandler/ALRPCObjects/ButtonName.h"
#include "../include/JSONHandler/ALRPCObjects/ButtonEventMode.h"

/*
  interface	RPC2Communication::Buttons
  version	1.2
  generated at	Wed Nov  7 11:25:50 2012
  source stamp	Wed Nov  7 09:31:20 2012
  author	robok0der
*/

namespace RPC2Communication
{
  namespace Buttons
  {

    class OnButtonEvent : public ::RPC2Communication::RPC2Notification
    {
    public:
    
      OnButtonEvent(const OnButtonEvent& c);
      OnButtonEvent(void);
    
      OnButtonEvent& operator =(const OnButtonEvent&);
    
      virtual ~OnButtonEvent(void);
    
      bool checkIntegrity(void);
    
// getters
      const ::AppLinkRPC::ButtonName& get_name(void);

      const ::AppLinkRPC::ButtonEventMode& get_mode(void);


// setters
      bool set_name(const ::AppLinkRPC::ButtonName& name);

      bool set_mode(const ::AppLinkRPC::ButtonEventMode& mode);


    private:

      friend class OnButtonEventMarshaller;

      ::AppLinkRPC::ButtonName name;
      ::AppLinkRPC::ButtonEventMode mode;

    };
  }
}

#endif
