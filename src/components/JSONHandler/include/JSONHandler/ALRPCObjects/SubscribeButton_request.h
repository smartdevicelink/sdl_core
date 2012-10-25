#ifndef SUBSCRIBEBUTTON_REQUEST_INCLUDE
#define SUBSCRIBEBUTTON_REQUEST_INCLUDE


#include "ButtonName.h"
#include "JSONHandler/ALRPCRequest.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Oct 25 04:31:05 2012
  source stamp	Wed Oct 24 14:57:16 2012
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
