#ifndef ONBUTTONEVENT_INCLUDE
#define ONBUTTONEVENT_INCLUDE


#include "ButtonEventMode.h"
#include "ButtonName.h"
#include "JSONHandler/ALRPCNotification.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Oct 25 06:32:04 2012
  source stamp	Thu Oct 25 06:28:28 2012
  author	robok0der
*/


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
  ButtonEventMode buttonEventMode;
};

#endif
