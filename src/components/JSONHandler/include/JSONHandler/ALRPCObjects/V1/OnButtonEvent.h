#ifndef NSAPPLINKRPC_ONBUTTONEVENT_INCLUDE
#define NSAPPLINKRPC_ONBUTTONEVENT_INCLUDE


#include "ButtonEventMode.h"
#include "ButtonName.h"
#include "JSONHandler/ALRPCNotification.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Dec 13 13:37:09 2012
  source stamp	Thu Dec 13 13:33:23 2012
  author	robok0der
*/

namespace NsAppLinkRPC
{

///  Notifies application of UP/DOWN events for buttons to which the application is subscribed.

  class OnButtonEvent : public ALRPCNotification
  {
  public:
  
    OnButtonEvent(const OnButtonEvent& c);
    OnButtonEvent(void);
    
    virtual ~OnButtonEvent(void);
  
    bool checkIntegrity(void);

    const ButtonName& get_buttonName(void) const;
    const ButtonEventMode& get_buttonEventMode(void) const;

    bool set_buttonName(const ButtonName& buttonName_);
    bool set_buttonEventMode(const ButtonEventMode& buttonEventMode_);

  private:
  
    friend class OnButtonEventMarshaller;

      ButtonName buttonName;

///  Indicates whether this is an UP or DOWN event.
      ButtonEventMode buttonEventMode;
  };

}

#endif
