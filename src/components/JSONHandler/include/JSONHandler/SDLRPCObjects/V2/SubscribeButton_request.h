//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSSMARTDEVICELINKKRPCV2_SUBSCRIBEBUTTON_REQUEST_INCLUDE
#define NSSMARTDEVICELINKKRPCV2_SUBSCRIBEBUTTON_REQUEST_INCLUDE


#include "ButtonName.h"
#include "JSONHandler/SDLRPCMessage.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	RC
*/

namespace NsSmartDeviceLinkRPCV2
{

/**
     Subscribes to built-in HMI buttons.
     The application will be notified by the OnButtonEvent and OnButtonPress.
     To unsubscribe the notifications, use unsubscribeButton.
*/

  class SubscribeButton_request : public NsSmartDeviceLinkRPC::SDLRPCMessage
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


///  Name of the button to subscribe.
      ButtonName buttonName;
  };

}

#endif
