#ifndef NSAPPLINKRPCV2_ONBUTTONPRESS_INCLUDE
#define NSAPPLINKRPCV2_ONBUTTONPRESS_INCLUDE


#include "ButtonName.h"
#include "ButtonPressMode.h"
#include "JSONHandler/ALRPCMessage.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Dec 13 14:18:29 2012
  source stamp	Thu Dec 13 14:18:27 2012
  author	robok0der
*/

namespace NsAppLinkRPCV2
{

///  Notifies application of LONG/SHORT press events for buttons to which the application is subscribed.

  class OnButtonPress : public NsAppLinkRPC::ALRPCMessage
  {
  public:
  
    OnButtonPress(const OnButtonPress& c);
    OnButtonPress(void);
    
    virtual ~OnButtonPress(void);
  
    bool checkIntegrity(void);

    const ButtonName& get_buttonName(void) const;
    const ButtonPressMode& get_buttonPressMode(void) const;
    unsigned int get_customButtonID(void) const;

    bool set_buttonName(const ButtonName& buttonName_);
    bool set_buttonPressMode(const ButtonPressMode& buttonPressMode_);
    bool set_customButtonID(unsigned int customButtonID_);

  private:
  
    friend class OnButtonPressMarshaller;

      ButtonName buttonName;

///  Indicates whether this is a LONG or SHORT button press event.
      ButtonPressMode buttonPressMode;

///  If ButtonName is “CUSTOM_BUTTON", this references the integer ID passed by a custom button. (e.g. softButton ID)
      unsigned int customButtonID;	//!<  (0,65536)
  };

}

#endif
