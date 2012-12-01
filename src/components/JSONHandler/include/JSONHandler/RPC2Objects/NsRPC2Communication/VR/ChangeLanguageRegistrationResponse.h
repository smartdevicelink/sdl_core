#ifndef NSRPC2COMMUNICATION_VR_CHANGELANGUAGEREGISTRATIONRESPONSE_INCLUDE
#define NSRPC2COMMUNICATION_VR_CHANGELANGUAGEREGISTRATIONRESPONSE_INCLUDE

#include "JSONHandler/RPC2Response.h"


/*
  interface	NsRPC2Communication::VR
  version	1.2
  generated at	Thu Nov 29 14:32:09 2012
  source stamp	Thu Nov 29 14:32:05 2012
  author	robok0der
*/

namespace NsRPC2Communication
{
  namespace VR
  {

    class ChangeLanguageRegistrationResponse : public ::NsRPC2Communication::RPC2Response
    {
    public:
    
      ChangeLanguageRegistrationResponse(const ChangeLanguageRegistrationResponse& c);
      ChangeLanguageRegistrationResponse(void);
    
      ChangeLanguageRegistrationResponse& operator =(const ChangeLanguageRegistrationResponse&);
    
      virtual ~ChangeLanguageRegistrationResponse(void);
    
      bool checkIntegrity(void);
    

    private:

      friend class ChangeLanguageRegistrationResponseMarshaller;


    };
  }
}

#endif
