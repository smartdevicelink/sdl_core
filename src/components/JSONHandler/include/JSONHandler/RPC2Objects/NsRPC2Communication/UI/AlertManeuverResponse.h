//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSRPC2COMMUNICATION_UI_ALERTMANEUVERRESPONSE_INCLUDE
#define NSRPC2COMMUNICATION_UI_ALERTMANEUVERRESPONSE_INCLUDE

#include "JSONHandler/RPC2Response.h"


/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
*/

namespace NsRPC2Communication
{
  namespace UI
  {

    class AlertManeuverResponse : public ::NsRPC2Communication::RPC2Response
    {
    public:
    
      AlertManeuverResponse(const AlertManeuverResponse& c);
      AlertManeuverResponse(void);
    
      AlertManeuverResponse& operator =(const AlertManeuverResponse&);
    
      virtual ~AlertManeuverResponse(void);
    
      bool checkIntegrity(void);
    

    private:

      friend class AlertManeuverResponseMarshaller;


    };
  }
}

#endif
