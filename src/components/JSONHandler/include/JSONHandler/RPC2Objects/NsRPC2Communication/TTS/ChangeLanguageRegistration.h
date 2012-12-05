#ifndef NSRPC2COMMUNICATION_TTS_CHANGELANGUAGEREGISTRATION_INCLUDE
#define NSRPC2COMMUNICATION_TTS_CHANGELANGUAGEREGISTRATION_INCLUDE

#include "JSONHandler/RPC2Request.h"

#include "../include/JSONHandler/ALRPCObjects/V1/Language.h"

/*
  interface	NsRPC2Communication::TTS
  version	1.2
  generated at	Tue Dec  4 16:38:13 2012
  source stamp	Tue Dec  4 16:37:04 2012
  author	robok0der
*/

namespace NsRPC2Communication
{
  namespace TTS
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
      const NsAppLinkRPC::Language& get_language(void);

      int get_appId(void);


// setters
      bool set_language(const NsAppLinkRPC::Language& language);

      bool set_appId(int appId);


    private:

      friend class ChangeLanguageRegistrationMarshaller;

      NsAppLinkRPC::Language language;
      int appId;

    };
  }
}

#endif
