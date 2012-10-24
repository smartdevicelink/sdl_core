#ifndef ONBUTTONPRESS_INCLUDE
#define ONBUTTONPRESS_INCLUDE

#include <string>

#include "ButtonName.h"
#include "ButtonPressMode.h"
#include "../../../JSONHandler/ALRPCNotification.hh"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/


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
  const std::string& get_customButtonName(void) const;

  bool set_buttonName(const ButtonName& buttonName_);
  bool set_buttonPressMode(const ButtonPressMode& buttonPressMode_);
  bool set_customButtonName(const std::string& customButtonName_);

private:

  friend class OnButtonPressMarshaller;

  ButtonName buttonName;
  ButtonPressMode buttonPressMode;
  std::string customButtonName;	//!< (500)
};

#endif
