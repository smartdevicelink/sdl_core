//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSRPC2COMMUNICATION_SMARTDEVICELINKCORE_ONAPPDEACTIVATEDRESPONSE_INCLUDE
#define NSRPC2COMMUNICATION_SMARTDEVICELINKCORE_ONAPPDEACTIVATEDRESPONSE_INCLUDE

#include "JSONHandler/RPC2Response.h"


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

    class OnAppDeactivatedResponse : public ::NsRPC2Communication::RPC2Response
    {
    public:
    
      OnAppDeactivatedResponse(const OnAppDeactivatedResponse& c);
      OnAppDeactivatedResponse(void);
    
      OnAppDeactivatedResponse& operator =(const OnAppDeactivatedResponse&);
    
      virtual ~OnAppDeactivatedResponse(void);
    
      bool checkIntegrity(void);
    

    private:

      friend class OnAppDeactivatedResponseMarshaller;


    };
  }
}

#endif
