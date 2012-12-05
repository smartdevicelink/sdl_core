#ifndef NSRPC2COMMUNICATION_UI_ONDEVICECHOSEN_INCLUDE
#define NSRPC2COMMUNICATION_UI_ONDEVICECHOSEN_INCLUDE

#include <string>
#include "JSONHandler/RPC2Notification.h"


/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Tue Dec  4 16:38:13 2012
  source stamp	Tue Dec  4 16:37:04 2012
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
