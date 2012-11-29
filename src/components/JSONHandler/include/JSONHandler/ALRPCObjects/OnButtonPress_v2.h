#ifndef NSAPPLINKRPC_ONBUTTONPRESS_V2_INCLUDE
#define NSAPPLINKRPC_ONBUTTONPRESS_V2_INCLUDE

#include <string>

#include "ButtonName_v2.h"
#include "ButtonPressMode.h"
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

///  Notifies application of LONG/SHORT press events for buttons to which the application is subscribed.

  class OnButtonPress_v2 : public ALRPCNotification
  {
  public:
  
    OnButtonPress_v2(const OnButtonPress_v2& c);
    OnButtonPress_v2(void);
    
    virtual ~OnButtonPress_v2(void);
  
    bool checkIntegrity(void);

    const ButtonName_v2& get_buttonName(void) const;
    const ButtonPressMode& get_buttonPressMode(void) const;
    const std::string& get_customButtonName(void) const;

    bool set_buttonName(const ButtonName_v2& buttonName_);
    bool set_buttonPressMode(const ButtonPressMode& buttonPressMode_);
    bool set_customButtonName(const std::string& customButtonName_);

  private:
  
    friend class OnButtonPress_v2Marshaller;

      ButtonName_v2 buttonName;

///  Indicates whether this is a LONG or SHORT button press event.
      ButtonPressMode buttonPressMode;

///  If ButtonName is “CUSTOM_BUTTON", this references the string passed by a custom button. (e.g. softButtonName)
      std::string customButtonName;	//!< (500)
  };

}

#endif
