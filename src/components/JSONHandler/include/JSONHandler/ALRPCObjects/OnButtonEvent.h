#ifndef ONBUTTONEVENT_INCLUDE
#define ONBUTTONEVENT_INCLUDE


#include "ButtonEventMode.h"
#include "ButtonName.h"
#include "../../../JSONHandler/ALRPCNotification.hh"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
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
  unsigned int get_customButtonID(void) const;

  bool set_buttonName(const ButtonName& buttonName_);
  bool set_buttonEventMode(const ButtonEventMode& buttonEventMode_);
  bool set_customButtonID(unsigned int customButtonID_);

private:

  friend class OnButtonEventMarshaller;

  ButtonName buttonName;
  ButtonEventMode buttonEventMode;
  unsigned int customButtonID;	//!<  (0,65536)
};

#endif
