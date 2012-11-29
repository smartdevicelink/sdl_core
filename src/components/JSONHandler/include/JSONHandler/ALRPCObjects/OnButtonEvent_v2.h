#ifndef NSAPPLINKRPC_ONBUTTONEVENT_V2_INCLUDE
#define NSAPPLINKRPC_ONBUTTONEVENT_V2_INCLUDE


#include "ButtonEventMode.h"
#include "ButtonName_v2.h"
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

///  Notifies application of UP/DOWN events for buttons to which the application is subscribed.

  class OnButtonEvent_v2 : public ALRPCNotification
  {
  public:
  
    OnButtonEvent_v2(const OnButtonEvent_v2& c);
    OnButtonEvent_v2(void);
    
    virtual ~OnButtonEvent_v2(void);
  
    bool checkIntegrity(void);

    const ButtonName_v2& get_buttonName(void) const;
    const ButtonEventMode& get_buttonEventMode(void) const;
    unsigned int get_customButtonID(void) const;

    bool set_buttonName(const ButtonName_v2& buttonName_);
    bool set_buttonEventMode(const ButtonEventMode& buttonEventMode_);
    bool set_customButtonID(unsigned int customButtonID_);

  private:
  
    friend class OnButtonEvent_v2Marshaller;

      ButtonName_v2 buttonName;

///  Indicates whether this is an UP or DOWN event.
      ButtonEventMode buttonEventMode;

///  If ButtonName is “CUSTOM_BUTTON", this references the integer ID passed by a custom button. (e.g. softButtonName)
      unsigned int customButtonID;	//!<  (0,65536)
  };

}

#endif
