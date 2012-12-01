#ifndef NSRPC2COMMUNICATION_UI_ONDEVICELISTUPDATED_INCLUDE
#define NSRPC2COMMUNICATION_UI_ONDEVICELISTUPDATED_INCLUDE

#include <string>
#include <vector>
#include "JSONHandler/RPC2Notification.h"


/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Thu Nov 29 14:32:09 2012
  source stamp	Thu Nov 29 14:32:05 2012
  author	robok0der
*/

namespace NsRPC2Communication
{
  namespace UI
  {

    class OnDeviceListUpdated : public ::NsRPC2Communication::RPC2Notification
    {
    public:
    
      OnDeviceListUpdated(const OnDeviceListUpdated& c);
      OnDeviceListUpdated(void);
    
      OnDeviceListUpdated& operator =(const OnDeviceListUpdated&);
    
      virtual ~OnDeviceListUpdated(void);
    
      bool checkIntegrity(void);
    
// getters
      const std::vector< std::string>& get_deviceList(void);


// setters
/// 1 <= size <= 100
      bool set_deviceList(const std::vector< std::string>& deviceList);


    private:

      friend class OnDeviceListUpdatedMarshaller;

      std::vector< std::string> deviceList;

    };
  }
}

#endif
