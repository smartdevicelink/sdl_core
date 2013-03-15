//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSRPC2COMMUNICATION_UI_ONSYSTEMCONTEXT_INCLUDE
#define NSRPC2COMMUNICATION_UI_ONSYSTEMCONTEXT_INCLUDE

#include "JSONHandler/RPC2Notification.h"

#include "../include/JSONHandler/SDLRPCObjects/V2/SystemContext.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	RC
*/

namespace NsRPC2Communication
{
  namespace UI
  {

    class OnSystemContext : public ::NsRPC2Communication::RPC2Notification
    {
    public:
    
      OnSystemContext(const OnSystemContext& c);
      OnSystemContext(void);
    
      OnSystemContext& operator =(const OnSystemContext&);
    
      virtual ~OnSystemContext(void);
    
      bool checkIntegrity(void);
    
// getters
      const NsSmartDeviceLinkRPCV2::SystemContext& get_systemContext(void);


// setters
      bool set_systemContext(const NsSmartDeviceLinkRPCV2::SystemContext& systemContext);


    private:

      friend class OnSystemContextMarshaller;

      NsSmartDeviceLinkRPCV2::SystemContext systemContext;

    };
  }
}

#endif
