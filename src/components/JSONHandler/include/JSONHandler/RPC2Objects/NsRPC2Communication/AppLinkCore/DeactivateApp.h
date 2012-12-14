#ifndef NSRPC2COMMUNICATION_APPLINKCORE_DEACTIVATEAPP_INCLUDE
#define NSRPC2COMMUNICATION_APPLINKCORE_DEACTIVATEAPP_INCLUDE

#include <string>
#include "JSONHandler/RPC2Request.h"

#include "../include/JSONHandler/ALRPCObjects/V1/DeactivateReason.h"

/*
  interface	NsRPC2Communication::AppLinkCore
  version	1.2
  generated at	Fri Dec 14 06:14:25 2012
  source stamp	Fri Dec 14 06:14:23 2012
  author	robok0der
*/

namespace NsRPC2Communication
{
  namespace AppLinkCore
  {

    class DeactivateApp : public ::NsRPC2Communication::RPC2Request
    {
    public:
    
      DeactivateApp(const DeactivateApp& c);
      DeactivateApp(void);
    
      DeactivateApp& operator =(const DeactivateApp&);
    
      virtual ~DeactivateApp(void);
    
      bool checkIntegrity(void);
    
// getters
      const std::string& get_appName(void);

      const NsAppLinkRPC::DeactivateReason& get_reason(void);

      int get_appId(void);


// setters
/// appName <= 100
      bool set_appName(const std::string& appName);

      bool set_reason(const NsAppLinkRPC::DeactivateReason& reason);

      bool set_appId(int appId);


    private:

      friend class DeactivateAppMarshaller;

      std::string appName;
      NsAppLinkRPC::DeactivateReason reason;
      int appId;

    };
  }
}

#endif
