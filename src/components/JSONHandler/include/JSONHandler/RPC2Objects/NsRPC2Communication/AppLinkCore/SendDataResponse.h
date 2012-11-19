#ifndef NSRPC2COMMUNICATION_APPLINKCORE_SENDDATARESPONSE_INCLUDE
#define NSRPC2COMMUNICATION_APPLINKCORE_SENDDATARESPONSE_INCLUDE

#include "JSONHandler/RPC2Response.h"


/*
  interface	NsRPC2Communication::AppLinkCore
  version	1.2
  generated at	Mon Nov 19 07:25:04 2012
  source stamp	Mon Nov 19 07:04:40 2012
  author	robok0der
*/

namespace NsRPC2Communication
{
  namespace AppLinkCore
  {

    class SendDataResponse : public ::RPC2Communication::RPC2Response
    {
    public:
    
      SendDataResponse(const SendDataResponse& c);
      SendDataResponse(void);
    
      SendDataResponse& operator =(const SendDataResponse&);
    
      virtual ~SendDataResponse(void);
    
      bool checkIntegrity(void);
    

    private:

      friend class SendDataResponseMarshaller;


    };
  }
}

#endif
