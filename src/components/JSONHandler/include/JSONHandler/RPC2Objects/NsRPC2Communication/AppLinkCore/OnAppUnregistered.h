#ifndef NSRPC2COMMUNICATION_APPLINKCORE_ONAPPUNREGISTERED_INCLUDE
#define NSRPC2COMMUNICATION_APPLINKCORE_ONAPPUNREGISTERED_INCLUDE

#include <string>
#include "JSONHandler/RPC2Notification.h"

#include "../include/JSONHandler/ALRPCObjects/AppInterfaceUnregisteredReason.h"

/*
  interface	NsRPC2Communication::AppLinkCore
  version	1.2
  generated at	Mon Nov 19 10:37:39 2012
  source stamp	Mon Nov 19 10:17:20 2012
  author	robok0der
*/

namespace NsRPC2Communication
{
  namespace AppLinkCore
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

      const NsAppLinkRPC::AppInterfaceUnregisteredReason* get_reason(void);

// setters
/// appName <= 100
      bool set_appName(const std::string& appName);

      bool set_reason(const NsAppLinkRPC::AppInterfaceUnregisteredReason& reason);

      void reset_reason(void);


    private:

      friend class OnAppUnregisteredMarshaller;

      std::string appName;
      NsAppLinkRPC::AppInterfaceUnregisteredReason* reason;

    };
  }
}

#endif
