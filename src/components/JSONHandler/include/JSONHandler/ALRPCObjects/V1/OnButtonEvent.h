#ifndef NSAPPLINKRPCV1_ONBUTTONEVENT_INCLUDE
#define NSAPPLINKRPCV1_ONBUTTONEVENT_INCLUDE


#include "ButtonEventMode.h"
#include "ButtonName.h"
#include "JSONHandler/ALRPCNotification.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Tue Dec  4 14:55:10 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

namespace NsAppLinkRPCV1
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
