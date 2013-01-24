#ifndef NSRPC2COMMUNICATION_APPLINKCORE_ONAPPREGISTERED_INCLUDE
#define NSRPC2COMMUNICATION_APPLINKCORE_ONAPPREGISTERED_INCLUDE

#include "JSONHandler/RPC2Notification.h"

#include "../include/JSONHandler/ALRPCObjects/V2/HMIApplication.h"

/*
  interface	NsRPC2Communication::AppLinkCore
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	robok0der
*/

namespace NsRPC2Communication
{
  namespace AppLinkCore
  {

    class OnAppRegistered : public ::NsRPC2Communication::RPC2Notification
    {
    public:
    
      OnAppRegistered(const OnAppRegistered& c);
      OnAppRegistered(void);
    
      OnAppRegistered& operator =(const OnAppRegistered&);
    
      virtual ~OnAppRegistered(void);
    
      bool checkIntegrity(void);
    
// getters
      const NsAppLinkRPCV2::HMIApplication& get_application(void);


// setters
      bool set_application(const NsAppLinkRPCV2::HMIApplication& application);


    private:

      friend class OnAppRegisteredMarshaller;

      NsAppLinkRPCV2::HMIApplication application;

    };
  }
}

#endif
