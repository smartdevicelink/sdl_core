#ifndef NSRPC2COMMUNICATION_APPLINKCORE_ONAPPUNREGISTERED_INCLUDE
#define NSRPC2COMMUNICATION_APPLINKCORE_ONAPPUNREGISTERED_INCLUDE

#include <string>
#include "JSONHandler/RPC2Notification.h"

#include "../include/JSONHandler/SDLRPCObjects/V2/AppInterfaceUnregisteredReason.h"

/*
  interface	NsRPC2Communication::AppLinkCore
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	robok0der
*/

namespace NsRPC2Communication
{
  namespace BasicCommunication
  {

    class OnAppUnregistered : public ::NsRPC2Communication::RPC2Notification
    {
    public:
    
      OnAppUnregistered(const OnAppUnregistered& c);
      OnAppUnregistered(void);
    
      OnAppUnregistered& operator =(const OnAppUnregistered&);
    
      virtual ~OnAppUnregistered(void);
    
      bool checkIntegrity(void);
    
// getters
      const std::string& get_appName(void);

      const NsSmartDeviceLinkRPCV2::AppInterfaceUnregisteredReason* get_reason(void);
      int get_appId(void);


// setters
/// appName <= 100
      bool set_appName(const std::string& appName);

      bool set_reason(const NsSmartDeviceLinkRPCV2::AppInterfaceUnregisteredReason& reason);

      void reset_reason(void);

      bool set_appId(int appId);


    private:

      friend class OnAppUnregisteredMarshaller;

      std::string appName;
      NsSmartDeviceLinkRPCV2::AppInterfaceUnregisteredReason* reason;
      int appId;

    };
  }
}

#endif
