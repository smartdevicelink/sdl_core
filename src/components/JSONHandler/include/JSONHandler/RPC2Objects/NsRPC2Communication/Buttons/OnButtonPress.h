//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSRPC2COMMUNICATION_BUTTONS_ONBUTTONPRESS_INCLUDE
#define NSRPC2COMMUNICATION_BUTTONS_ONBUTTONPRESS_INCLUDE

#include "JSONHandler/RPC2Notification.h"

#include "../include/JSONHandler/SDLRPCObjects/V2/ButtonName.h"
#include "../include/JSONHandler/SDLRPCObjects/V2/ButtonPressMode.h"

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

    class OnButtonPress : public ::NsRPC2Communication::RPC2Notification
    {
    public:
    
      OnButtonPress(const OnButtonPress& c);
      OnButtonPress(void);
    
      OnButtonPress& operator =(const OnButtonPress&);
    
      virtual ~OnButtonPress(void);
    
      bool checkIntegrity(void);
    
// getters
      const NsSmartDeviceLinkRPCV2::ButtonName& get_name(void);

      const NsSmartDeviceLinkRPCV2::ButtonPressMode& get_mode(void);

      const unsigned int* get_customButtonID(void);

// setters
      bool set_name(const NsSmartDeviceLinkRPCV2::ButtonName& name);

      bool set_mode(const NsSmartDeviceLinkRPCV2::ButtonPressMode& mode);

/// customButtonID <= 65536
      bool set_customButtonID(const unsigned int& customButtonID);

      void reset_customButtonID(void);


    private:

      friend class OnButtonPressMarshaller;

      NsSmartDeviceLinkRPCV2::ButtonName name;
      NsSmartDeviceLinkRPCV2::ButtonPressMode mode;
      unsigned int* customButtonID;

    };
  }
}

#endif
