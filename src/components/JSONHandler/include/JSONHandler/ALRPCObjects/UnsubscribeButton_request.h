#ifndef UNSUBSCRIBEBUTTON_REQUEST_INCLUDE
#define UNSUBSCRIBEBUTTON_REQUEST_INCLUDE


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


///  Unsubscribes from built-in HMI buttons.

class UnsubscribeButton_request : public ALRPCRequest
{
public:

  UnsubscribeButton_request(const UnsubscribeButton_request& c);
  UnsubscribeButton_request(void);
  
  virtual ~UnsubscribeButton_request(void);

  bool checkIntegrity(void);

  const ButtonName& get_buttonName(void) const;

  bool set_buttonName(const ButtonName& buttonName_);

private:

  friend class UnsubscribeButton_requestMarshaller;

  ButtonName buttonName;
};

#endif
