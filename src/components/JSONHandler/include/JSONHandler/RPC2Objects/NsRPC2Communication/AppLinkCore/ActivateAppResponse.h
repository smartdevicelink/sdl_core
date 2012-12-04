#ifndef NSRPC2COMMUNICATION_APPLINKCORE_ACTIVATEAPPRESPONSE_INCLUDE
#define NSRPC2COMMUNICATION_APPLINKCORE_ACTIVATEAPPRESPONSE_INCLUDE

#include "JSONHandler/RPC2Response.h"


/*
  interface	NsRPC2Communication::AppLinkCore
  version	1.2
  generated at	Tue Dec  4 15:06:30 2012
  source stamp	Tue Dec  4 14:21:32 2012
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
