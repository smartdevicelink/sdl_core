#ifndef SENDDATARESPONSE_H
#define SENDDATARESPONSE_H

#include "JSONHandler/RPC2Response.h"


/*
  interface	RPC2Communication::AppLinkCore
  version	1.2
  generated at	Wed Nov  7 11:26:14 2012
  source stamp	Wed Nov  7 09:29:07 2012
  author	robok0der
*/

namespace RPC2Communication
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

#endif // SENDDATARESPONSE_H
