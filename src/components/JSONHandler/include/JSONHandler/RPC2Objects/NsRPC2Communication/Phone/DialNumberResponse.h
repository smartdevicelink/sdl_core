#ifndef NSRPC2COMMUNICATION_PHONE_DIALNUMBERRESPONSE_INCLUDE
#define NSRPC2COMMUNICATION_PHONE_DIALNUMBERRESPONSE_INCLUDE

#include "JSONHandler/RPC2Response.h"


/*
  interface	NsRPC2Communication::Phone
  version	2.0
  generated at	Fri Dec 14 06:14:25 2012
  source stamp	Fri Dec 14 06:14:23 2012
  author	robok0der
*/

namespace NsRPC2Communication
{
  namespace Phone
  {

    class DialNumberResponse : public ::NsRPC2Communication::RPC2Response
    {
    public:
    
      DialNumberResponse(const DialNumberResponse& c);
      DialNumberResponse(void);
    
      DialNumberResponse& operator =(const DialNumberResponse&);
    
      virtual ~DialNumberResponse(void);
    
      bool checkIntegrity(void);
    

    private:

      friend class DialNumberResponseMarshaller;


    };
  }
}

#endif
