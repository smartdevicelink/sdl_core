//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSRPC2COMMUNICATION_SMARTDEVICELINKCORE_ONAPPREGISTERED_INCLUDE
#define NSRPC2COMMUNICATION_SMARTDEVICELINKCORE_ONAPPREGISTERED_INCLUDE

#include "JSONHandler/RPC2Notification.h"

#include "../include/JSONHandler/SDLRPCObjects/V2/HMIApplication.h"

/*
  interface	NsRPC2Communication::SmartDeviceLinkCore
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
*/

namespace NsRPC2Communication
{
  namespace BasicCommunication
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
      const NsSmartDeviceLinkRPCV2::HMIApplication& get_application(void);


// setters
      bool set_application(const NsSmartDeviceLinkRPCV2::HMIApplication& application);


    private:

      friend class OnAppRegisteredMarshaller;

      NsSmartDeviceLinkRPCV2::HMIApplication application;

    };
  }
}

#endif
