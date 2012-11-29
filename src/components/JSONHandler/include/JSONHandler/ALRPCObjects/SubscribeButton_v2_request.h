#ifndef NSAPPLINKRPC_SUBSCRIBEBUTTON_V2_REQUEST_INCLUDE
#define NSAPPLINKRPC_SUBSCRIBEBUTTON_V2_REQUEST_INCLUDE


#include "ButtonName_v2.h"
#include "JSONHandler/ALRPCRequest.h"


/*
  interface	Ford Sync RAPI
  version	1.2 / 2.0O
  date		2011-05-17 / 2012-11-02
  generated at	Thu Nov 29 14:49:08 2012
  source stamp	Thu Nov 29 06:50:10 2012
  author	robok0der
*/

namespace NsAppLinkRPC
{

/**
     Subscribes to built-in HMI buttons.
     The application will be notified by the OnButtonEvent and OnButtonPress.
     To unsubscribe the notifications, use unsubscribeButton.
*/

  class SubscribeButton_v2_request : public ALRPCRequest
  {
  public:
  
    SubscribeButton_v2_request(const SubscribeButton_v2_request& c);
    SubscribeButton_v2_request(void);
    
    virtual ~SubscribeButton_v2_request(void);
  
    bool checkIntegrity(void);

    const ButtonName_v2& get_buttonName(void) const;

    bool set_buttonName(const ButtonName_v2& buttonName_);

  private:
  
    friend class SubscribeButton_v2_requestMarshaller;


///  Name of the button to subscribe.
      ButtonName_v2 buttonName;
  };

}

#endif
