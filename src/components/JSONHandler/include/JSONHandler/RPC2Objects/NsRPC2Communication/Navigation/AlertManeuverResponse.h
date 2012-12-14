#ifndef NSRPC2COMMUNICATION_NAVIGATION_ALERTMANEUVERRESPONSE_INCLUDE
#define NSRPC2COMMUNICATION_NAVIGATION_ALERTMANEUVERRESPONSE_INCLUDE

#include "JSONHandler/RPC2Response.h"


/*
  interface	NsRPC2Communication::Navigation
  version	2.0
  generated at	Fri Dec 14 06:14:25 2012
  source stamp	Fri Dec 14 06:14:23 2012
  author	robok0der
*/

namespace NsRPC2Communication
{
  namespace Navigation
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
