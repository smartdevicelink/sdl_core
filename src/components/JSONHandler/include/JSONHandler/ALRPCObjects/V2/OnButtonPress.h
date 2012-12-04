#ifndef NSAPPLINKRPCV2_ONBUTTONPRESS_INCLUDE
#define NSAPPLINKRPCV2_ONBUTTONPRESS_INCLUDE

#include <string>

#include "ButtonName.h"
#include "ButtonPressMode.h"
#include "JSONHandler/ALRPC2Message.hh"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Tue Dec  4 14:30:13 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

namespace NsAppLinkRPCV2
{

///  Notifies application of LONG/SHORT press events for buttons to which the application is subscribed.

  class OnButtonPress : public ALRPC2Message
  {
  public:
  
    OnButtonPress(const OnButtonPress& c);
    OnButtonPress(void);
    
    virtual ~OnButtonPress(void);
  
    bool checkIntegrity(void);

    const ButtonName& get_buttonName(void) const;
    const ButtonPressMode& get_buttonPressMode(void) const;
    const std::string& get_customButtonName(void) const;

    bool set_buttonName(const ButtonName& buttonName_);
    bool set_buttonPressMode(const ButtonPressMode& buttonPressMode_);
    bool set_customButtonName(const std::string& customButtonName_);

  private:
  
    friend class OnButtonPressMarshaller;

      ButtonName buttonName;

///  Indicates whether this is a LONG or SHORT button press event.
      ButtonPressMode buttonPressMode;

///  If ButtonName is “CUSTOM_BUTTON", this references the string passed by a custom button. (e.g. softButtonName)
      std::string customButtonName;	//!< (500)
  };

}

#endif
