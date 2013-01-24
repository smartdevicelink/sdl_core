#ifndef NSRPC2COMMUNICATION_UI_GETLANGUAGERESPONSE_INCLUDE
#define NSRPC2COMMUNICATION_UI_GETLANGUAGERESPONSE_INCLUDE

#include "JSONHandler/RPC2Response.h"

#include "../include/JSONHandler/ALRPCObjects/V2/Language.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
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
      const NsAppLinkRPCV2::Language& get_hmiDisplayLanguage(void);


// setters
      bool set_hmiDisplayLanguage(const NsAppLinkRPCV2::Language& hmiDisplayLanguage);


    private:

      friend class GetLanguageResponseMarshaller;

      NsAppLinkRPCV2::Language hmiDisplayLanguage;

    };
  }
}

#endif
