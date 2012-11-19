#ifndef NSRPC2COMMUNICATION_BUTTONS_ONBUTTONPRESS_INCLUDE
#define NSRPC2COMMUNICATION_BUTTONS_ONBUTTONPRESS_INCLUDE

#include "JSONHandler/RPC2Notification.h"

#include "../include/JSONHandler/ALRPCObjects/ButtonName.h"
#include "../include/JSONHandler/ALRPCObjects/ButtonPressMode.h"

/*
  interface	NsRPC2Communication::Buttons
  version	1.2
  generated at	Mon Nov 19 12:18:27 2012
  source stamp	Mon Nov 19 10:17:20 2012
  author	robok0der
*/

namespace NsRPC2Communication
{
  namespace Buttons
  {

    class OnButtonPress : public ::NsRPC2Communication::RPC2Notification
    {
    public:
    
      OnButtonPress(const OnButtonPress& c);
      OnButtonPress(void);
    
      OnButtonPress& operator =(const OnButtonPress&);
    
      virtual ~OnButtonPress(void);
    
      bool checkIntegrity(void);
    
// getters
      const NsAppLinkRPC::ButtonName& get_name(void);

      const NsAppLinkRPC::ButtonPressMode& get_mode(void);


// setters
      bool set_name(const NsAppLinkRPC::ButtonName& name);

      bool set_mode(const NsAppLinkRPC::ButtonPressMode& mode);


    private:

      friend class OnButtonPressMarshaller;

      NsAppLinkRPC::ButtonName name;
      NsAppLinkRPC::ButtonPressMode mode;

    };
  }
}

#endif
