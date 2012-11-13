#ifndef RPC2COMMUNICATION_BUTTONS_ONBUTTONPRESS_INCLUDE
#define RPC2COMMUNICATION_BUTTONS_ONBUTTONPRESS_INCLUDE

#include "JSONHandler/RPC2Notification.h"

#include "../include/JSONHandler/ALRPCObjects/ButtonName.h"
#include "../include/JSONHandler/ALRPCObjects/ButtonPressMode.h"

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

    class OnButtonPress : public ::RPC2Communication::RPC2Notification
    {
    public:
    
      OnButtonPress(const OnButtonPress& c);
      OnButtonPress(void);
    
      OnButtonPress& operator =(const OnButtonPress&);
    
      virtual ~OnButtonPress(void);
    
      bool checkIntegrity(void);
    
// getters
      const ::AppLinkRPC::ButtonName& get_name(void);

      const ::AppLinkRPC::ButtonPressMode& get_mode(void);


// setters
      bool set_name(const ::AppLinkRPC::ButtonName& name);

      bool set_mode(const ::AppLinkRPC::ButtonPressMode& mode);


    private:

      friend class OnButtonPressMarshaller;

      ::AppLinkRPC::ButtonName name;
      ::AppLinkRPC::ButtonPressMode mode;

    };
  }
}

#endif
