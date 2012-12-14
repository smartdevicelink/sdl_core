#ifndef NSAPPLINKRPC_UNSUBSCRIBEBUTTON_REQUEST_INCLUDE
#define NSAPPLINKRPC_UNSUBSCRIBEBUTTON_REQUEST_INCLUDE


#include "ButtonName.h"
#include "JSONHandler/ALRPCRequest.h"


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

}

#endif
