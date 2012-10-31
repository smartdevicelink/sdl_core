#ifndef UNSUBSCRIBEBUTTON_REQUEST_INCLUDE
#define UNSUBSCRIBEBUTTON_REQUEST_INCLUDE


#include "ButtonName.h"
#include "JSONHandler/ALRPCRequest.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Tue Oct 30 08:29:32 2012
  source stamp	Thu Oct 25 06:49:27 2012
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


///  Name of the button to unsubscribe.
    ButtonName buttonName;
};

#endif
