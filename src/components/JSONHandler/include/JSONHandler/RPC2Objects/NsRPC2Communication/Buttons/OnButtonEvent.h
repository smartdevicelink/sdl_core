//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSRPC2COMMUNICATION_BUTTONS_ONBUTTONEVENT_INCLUDE
#define NSRPC2COMMUNICATION_BUTTONS_ONBUTTONEVENT_INCLUDE

#include "JSONHandler/RPC2Notification.h"

#include "../include/JSONHandler/SDLRPCObjects/V2/ButtonName.h"
#include "../include/JSONHandler/SDLRPCObjects/V2/ButtonEventMode.h"

/*
  interface	NsRPC2Communication::Buttons
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
*/

namespace NsRPC2Communication
{
  namespace Buttons
  {

    class OnButtonEvent : public ::NsRPC2Communication::RPC2Notification
    {
    public:
    
      OnButtonEvent(const OnButtonEvent& c);
      OnButtonEvent(void);
    
      OnButtonEvent& operator =(const OnButtonEvent&);
    
      virtual ~OnButtonEvent(void);
    
      bool checkIntegrity(void);
    
// getters
      const NsSmartDeviceLinkRPCV2::ButtonName& get_name(void);

      const NsSmartDeviceLinkRPCV2::ButtonEventMode& get_mode(void);

      const unsigned int* get_customButtonID(void);

// setters
      bool set_name(const NsSmartDeviceLinkRPCV2::ButtonName& name);

      bool set_mode(const NsSmartDeviceLinkRPCV2::ButtonEventMode& mode);

/// customButtonID <= 65536
      bool set_customButtonID(const unsigned int& customButtonID);

      void reset_customButtonID(void);


    private:

      friend class OnButtonEventMarshaller;

      NsSmartDeviceLinkRPCV2::ButtonName name;
      NsSmartDeviceLinkRPCV2::ButtonEventMode mode;
      unsigned int* customButtonID;

    };
  }
}

#endif
