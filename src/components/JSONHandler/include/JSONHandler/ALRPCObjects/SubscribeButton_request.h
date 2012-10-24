#ifndef SUBSCRIBEBUTTON_REQUEST_INCLUDE
#define SUBSCRIBEBUTTON_REQUEST_INCLUDE


#include "ButtonName.h"
#include "../../../JSONHandler/ALRPCRequest.hh"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/


/**
     Subscribes to built-in HMI buttons.
     The application will be notified by the OnButtonEvent and OnButtonPress.
     To unsubscribe the notifications, use unsubscribeButton.
*/

class SubscribeButton_request : public ALRPCRequest
{
public:

  SubscribeButton_request(const SubscribeButton_request& c);
  SubscribeButton_request(void);
  
  virtual ~SubscribeButton_request(void);

  bool checkIntegrity(void);

  const ButtonName& get_buttonName(void) const;

  bool set_buttonName(const ButtonName& buttonName_);

private:

  friend class SubscribeButton_requestMarshaller;

  ButtonName buttonName;
};

#endif
