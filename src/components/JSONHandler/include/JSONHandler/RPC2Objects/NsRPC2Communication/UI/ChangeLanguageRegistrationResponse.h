#ifndef NSRPC2COMMUNICATION_UI_CHANGELANGUAGEREGISTRATIONRESPONSE_INCLUDE
#define NSRPC2COMMUNICATION_UI_CHANGELANGUAGEREGISTRATIONRESPONSE_INCLUDE

#include "JSONHandler/RPC2Response.h"


/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Tue Dec  4 16:38:13 2012
  source stamp	Tue Dec  4 16:37:04 2012
  author	robok0der
*/

namespace NsRPC2Communication
{
  namespace UI
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
