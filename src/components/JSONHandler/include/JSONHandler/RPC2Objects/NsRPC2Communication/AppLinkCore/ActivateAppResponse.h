#ifndef NSRPC2COMMUNICATION_APPLINKCORE_ACTIVATEAPPRESPONSE_INCLUDE
#define NSRPC2COMMUNICATION_APPLINKCORE_ACTIVATEAPPRESPONSE_INCLUDE

#include "JSONHandler/RPC2Response.h"


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

    class ActivateAppResponse : public ::NsRPC2Communication::RPC2Response
    {
    public:
    
      ActivateAppResponse(const ActivateAppResponse& c);
      ActivateAppResponse(void);
    
      ActivateAppResponse& operator =(const ActivateAppResponse&);
    
      virtual ~ActivateAppResponse(void);
    
      bool checkIntegrity(void);
    

    private:

      friend class ActivateAppResponseMarshaller;


    };
  }
}

#endif
