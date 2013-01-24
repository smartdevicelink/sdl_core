#ifndef NSAPPLINKRPC_ONBUTTONPRESS_INCLUDE
#define NSAPPLINKRPC_ONBUTTONPRESS_INCLUDE


#include "ButtonName.h"
#include "ButtonPressMode.h"
#include "JSONHandler/ALRPCNotification.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Jan 24 06:36:21 2013
  source stamp	Thu Jan 24 06:35:34 2013
  author	robok0der
*/

namespace NsAppLinkRPC
{

///  Notifies application of LONG/SHORT press events for buttons to which the application is subscribed.

  class OnButtonPress : public ALRPCNotification
  {
  public:
  
    OnButtonPress(const OnButtonPress& c);
    OnButtonPress(void);
    
    virtual ~OnButtonPress(void);
  
    bool checkIntegrity(void);

    const ButtonName& get_buttonName(void) const;
    const ButtonPressMode& get_buttonPressMode(void) const;

    bool set_buttonName(const ButtonName& buttonName_);
    bool set_buttonPressMode(const ButtonPressMode& buttonPressMode_);

  private:
  
    friend class OnButtonPressMarshaller;

      ButtonName buttonName;

///  Indicates whether this is a LONG or SHORT button press event.
      ButtonPressMode buttonPressMode;
  };

}

#endif
