#ifndef NSRPC2COMMUNICATION_APPLINKCORE_SENDDATARESPONSE_INCLUDE
#define NSRPC2COMMUNICATION_APPLINKCORE_SENDDATARESPONSE_INCLUDE

#include "JSONHandler/RPC2Response.h"


/*
  interface	NsRPC2Communication::AppLinkCore
  version	1.2
  generated at	Tue Dec  4 16:38:13 2012
  source stamp	Tue Dec  4 16:37:04 2012
  author	robok0der
*/

namespace NsRPC2Communication
{
  namespace AppLinkCore
  {

    class SendDataResponse : public ::NsRPC2Communication::RPC2Response
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
