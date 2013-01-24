#ifndef NSRPC2COMMUNICATION_BUTTONS_ONBUTTONPRESS_INCLUDE
#define NSRPC2COMMUNICATION_BUTTONS_ONBUTTONPRESS_INCLUDE

#include "JSONHandler/RPC2Notification.h"

#include "../include/JSONHandler/ALRPCObjects/V2/ButtonName.h"
#include "../include/JSONHandler/ALRPCObjects/V2/ButtonPressMode.h"

/*
  interface	NsRPC2Communication::Buttons
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	robok0der
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
      const NsAppLinkRPCV2::ButtonName& get_name(void);

      const NsAppLinkRPCV2::ButtonPressMode& get_mode(void);

      const unsigned int* get_customButtonID(void);

// setters
      bool set_name(const NsAppLinkRPCV2::ButtonName& name);

      bool set_mode(const NsAppLinkRPCV2::ButtonPressMode& mode);

/// customButtonID <= 65536
      bool set_customButtonID(const unsigned int& customButtonID);

      void reset_customButtonID(void);


    private:

      friend class OnButtonPressMarshaller;

      NsAppLinkRPCV2::ButtonName name;
      NsAppLinkRPCV2::ButtonPressMode mode;
      unsigned int* customButtonID;

    };
  }
}

#endif
