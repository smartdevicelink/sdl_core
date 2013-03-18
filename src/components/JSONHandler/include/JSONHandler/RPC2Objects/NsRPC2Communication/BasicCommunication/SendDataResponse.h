//
// Copiright (c) 2013 Ford Motor Company
//

#ifndef NSRPC2COMMUNICATION_SMARTDEVICELINKCORE_SENDDATARESPONSE_INCLUDE
#define NSRPC2COMMUNICATION_SMARTDEVICELINKCORE_SENDDATARESPONSE_INCLUDE

#include "JSONHandler/RPC2Response.h"


/*
  interface	NsRPC2Communication::SmartDeviceLinkCore
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	RC
*/

namespace NsRPC2Communication
{
  namespace BasicCommunication
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
