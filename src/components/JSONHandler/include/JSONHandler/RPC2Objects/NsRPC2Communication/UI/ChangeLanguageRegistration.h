#ifndef NSRPC2COMMUNICATION_UI_CHANGELANGUAGEREGISTRATION_INCLUDE
#define NSRPC2COMMUNICATION_UI_CHANGELANGUAGEREGISTRATION_INCLUDE

#include "JSONHandler/RPC2Request.h"

#include "../include/JSONHandler/ALRPCObjects/Language.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Thu Nov 29 14:32:09 2012
  source stamp	Thu Nov 29 14:32:05 2012
  author	robok0der
*/

namespace NsRPC2Communication
{
  namespace UI
  {

    class ChangeLanguageRegistration : public ::NsRPC2Communication::RPC2Request
    {
    public:
    
      ChangeLanguageRegistration(const ChangeLanguageRegistration& c);
      ChangeLanguageRegistration(void);
    
      ChangeLanguageRegistration& operator =(const ChangeLanguageRegistration&);
    
      virtual ~ChangeLanguageRegistration(void);
    
      bool checkIntegrity(void);
    
// getters
      const NsAppLinkRPC::Language& get_hmiDisplayLanguage(void);

      int get_appId(void);


// setters
      bool set_hmiDisplayLanguage(const NsAppLinkRPC::Language& hmiDisplayLanguage);

      bool set_appId(int appId);


    private:

      friend class ChangeLanguageRegistrationMarshaller;

      NsAppLinkRPC::Language hmiDisplayLanguage;
      int appId;

    };
  }
}

#endif
