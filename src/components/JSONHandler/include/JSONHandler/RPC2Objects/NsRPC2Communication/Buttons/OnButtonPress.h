#ifndef NSRPC2COMMUNICATION_BUTTONS_ONBUTTONPRESS_INCLUDE
#define NSRPC2COMMUNICATION_BUTTONS_ONBUTTONPRESS_INCLUDE

#include <string>
#include "JSONHandler/RPC2Notification.h"

#include "../include/JSONHandler/ALRPCObjects/V1/ButtonName.h"
#include "../include/JSONHandler/ALRPCObjects/V1/ButtonPressMode.h"

/*
  interface	NsRPC2Communication::Buttons
  version	1.2
  generated at	Tue Dec  4 16:38:13 2012
  source stamp	Tue Dec  4 16:37:04 2012
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

      const std::string* get_customButtonName(void);

// setters
      bool set_name(const NsAppLinkRPC::ButtonName& name);

      bool set_mode(const NsAppLinkRPC::ButtonPressMode& mode);

/// customButtonName <= 500
      bool set_customButtonName(const std::string& customButtonName);

      void reset_customButtonName(void);


    private:

      friend class OnButtonPressMarshaller;

      NsAppLinkRPC::ButtonName name;
      NsAppLinkRPC::ButtonPressMode mode;
      std::string* customButtonName;

    };
  }
}

#endif
