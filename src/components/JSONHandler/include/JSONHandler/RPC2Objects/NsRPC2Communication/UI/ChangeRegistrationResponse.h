#ifndef NSRPC2COMMUNICATION_UI_CHANGEREGISTRATIONRESPONSE_INCLUDE
#define NSRPC2COMMUNICATION_UI_CHANGEREGISTRATIONRESPONSE_INCLUDE

#include "JSONHandler/RPC2Response.h"


/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Fri Dec 14 06:14:25 2012
  source stamp	Fri Dec 14 06:14:23 2012
  author	robok0der
*/

namespace NsRPC2Communication
{
  namespace UI
  {

    class ChangeRegistrationResponse : public ::NsRPC2Communication::RPC2Response
    {
    public:
    
      ChangeRegistrationResponse(const ChangeRegistrationResponse& c);
      ChangeRegistrationResponse(void);
    
      ChangeRegistrationResponse& operator =(const ChangeRegistrationResponse&);
    
      virtual ~ChangeRegistrationResponse(void);
    
      bool checkIntegrity(void);
    

    private:

      friend class ChangeRegistrationResponseMarshaller;


    };
  }
}

#endif
