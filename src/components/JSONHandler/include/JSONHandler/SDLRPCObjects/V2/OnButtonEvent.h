#ifndef NSAPPLINKRPCV2_ONBUTTONEVENT_INCLUDE
#define NSAPPLINKRPCV2_ONBUTTONEVENT_INCLUDE


#include "ButtonEventMode.h"
#include "ButtonName.h"
#include "JSONHandler/SDLRPCMessage.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	robok0der
*/

namespace NsSmartDeviceLinkRPCV2
{

///  Notifies application of UP/DOWN events for buttons to which the application is subscribed.

  class OnButtonEvent : public NsSmartDeviceLinkRPC::SDLRPCMessage
  {
  public:
  
    OnButtonEvent(const OnButtonEvent& c);
    OnButtonEvent(void);
    
    virtual ~OnButtonEvent(void);
  
    bool checkIntegrity(void);

    const ButtonName& get_buttonName(void) const;
    const ButtonEventMode& get_buttonEventMode(void) const;
    unsigned int get_customButtonID(void) const;

    bool set_buttonName(const ButtonName& buttonName_);
    bool set_buttonEventMode(const ButtonEventMode& buttonEventMode_);
    bool set_customButtonID(unsigned int customButtonID_);

  private:
  
    friend class OnButtonEventMarshaller;

      ButtonName buttonName;

///  Indicates whether this is an UP or DOWN event.
      ButtonEventMode buttonEventMode;

///  If ButtonName is “CUSTOM_BUTTON", this references the integer ID passed by a custom button. (e.g. softButton ID)
      unsigned int customButtonID;	//!<  (0,65536)
  };

}

#endif
