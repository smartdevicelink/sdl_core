#ifndef NSRPC2COMMUNICATION_UI_ONDEVICECHOSEN_INCLUDE
#define NSRPC2COMMUNICATION_UI_ONDEVICECHOSEN_INCLUDE

#include <string>
#include "JSONHandler/RPC2Notification.h"


/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	robok0der
*/

namespace NsRPC2Communication
{
  namespace UI
  {

    class OnDeviceChosen : public ::NsRPC2Communication::RPC2Notification
    {
    public:
    
      OnDeviceChosen(const OnDeviceChosen& c);
      OnDeviceChosen(void);
    
      OnDeviceChosen& operator =(const OnDeviceChosen&);
    
      virtual ~OnDeviceChosen(void);
    
      bool checkIntegrity(void);
    
// getters
      const std::string& get_deviceName(void);


// setters
      bool set_deviceName(const std::string& deviceName);


    private:

      friend class OnDeviceChosenMarshaller;

      std::string deviceName;

    };
  }
}

#endif
