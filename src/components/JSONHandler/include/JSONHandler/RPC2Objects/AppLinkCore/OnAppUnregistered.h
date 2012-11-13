#ifndef RPC2COMMUNICATION_APPLINKCORE_ONAPPUNREGISTERED_INCLUDE
#define RPC2COMMUNICATION_APPLINKCORE_ONAPPUNREGISTERED_INCLUDE

#include <string>
#include "JSONHandler/RPC2Notification.h"

#include "../include/JSONHandler/ALRPCObjects/AppInterfaceUnregisteredReason.h"

/*
  interface	RPC2Communication::AppLinkCore
  version	1.2
  generated at	Wed Nov  7 11:25:43 2012
  source stamp	Wed Nov  7 09:35:35 2012
  author	robok0der
*/

namespace RPC2Communication
{
  namespace AppLinkCore
  {

    class OnAppUnregistered : public ::RPC2Communication::RPC2Notification
    {
    public:
    
      OnAppUnregistered(const OnAppUnregistered& c);
      OnAppUnregistered(void);
    
      OnAppUnregistered& operator =(const OnAppUnregistered&);
    
      virtual ~OnAppUnregistered(void);
    
      bool checkIntegrity(void);
    
// getters
      const std::string& get_appName(void);

  const     ::AppLinkRPC::AppInterfaceUnregisteredReason* get_reason(void);

// setters
/// appName <= 100
      bool set_appName(const std::string& appName);

      bool set_reason(const ::AppLinkRPC::AppInterfaceUnregisteredReason& reason);

      void reset_reason(void);


    private:

      friend class OnAppUnregisteredMarshaller;

      std::string appName;
      ::AppLinkRPC::AppInterfaceUnregisteredReason* reason;

    };
  }
}

#endif
