#ifndef NSAPPLINKRPC_UNSUBSCRIBEBUTTON_REQUEST_INCLUDE
#define NSAPPLINKRPC_UNSUBSCRIBEBUTTON_REQUEST_INCLUDE


#include "ButtonName.h"
#include "JSONHandler/SDLRPCRequest.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Jan 24 06:36:21 2013
  source stamp	Thu Jan 24 06:35:34 2013
  author	robok0der
*/

namespace NsSmartDeviceLinkRPC
{

///  Unsubscribes from built-in HMI buttons.

  class UnsubscribeButton_request : public SDLRPCRequest
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
