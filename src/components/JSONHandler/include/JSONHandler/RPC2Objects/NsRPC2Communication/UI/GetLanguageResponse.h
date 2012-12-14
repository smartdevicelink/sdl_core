#ifndef NSRPC2COMMUNICATION_UI_GETLANGUAGERESPONSE_INCLUDE
#define NSRPC2COMMUNICATION_UI_GETLANGUAGERESPONSE_INCLUDE

#include "JSONHandler/RPC2Response.h"

#include "../include/JSONHandler/ALRPCObjects/V1/Language.h"

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

    class GetLanguageResponse : public ::NsRPC2Communication::RPC2Response
    {
    public:
    
      GetLanguageResponse(const GetLanguageResponse& c);
      GetLanguageResponse(void);
    
      GetLanguageResponse& operator =(const GetLanguageResponse&);
    
      virtual ~GetLanguageResponse(void);
    
      bool checkIntegrity(void);
    
// getters
      const NsAppLinkRPC::Language& get_hmiDisplayLanguage(void);


// setters
      bool set_hmiDisplayLanguage(const NsAppLinkRPC::Language& hmiDisplayLanguage);


    private:

      friend class GetLanguageResponseMarshaller;

      NsAppLinkRPC::Language hmiDisplayLanguage;

    };
  }
}

#endif
