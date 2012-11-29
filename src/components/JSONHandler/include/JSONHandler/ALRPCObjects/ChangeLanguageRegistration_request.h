#ifndef NSAPPLINKRPC_CHANGELANGUAGEREGISTRATION_REQUEST_INCLUDE
#define NSAPPLINKRPC_CHANGELANGUAGEREGISTRATION_REQUEST_INCLUDE


#include "Language_v2.h"
#include "JSONHandler/ALRPCRequest.h"


/*
  interface	Ford Sync RAPI
  version	1.2 / 2.0O
  date		2011-05-17 / 2012-11-02
  generated at	Thu Nov 29 14:49:08 2012
  source stamp	Thu Nov 29 06:50:10 2012
  author	robok0der
*/

namespace NsAppLinkRPC
{

  class ChangeLanguageRegistration_request : public ALRPCRequest
  {
  public:
  
    ChangeLanguageRegistration_request(const ChangeLanguageRegistration_request& c);
    ChangeLanguageRegistration_request(void);
    
    virtual ~ChangeLanguageRegistration_request(void);
  
    ChangeLanguageRegistration_request& operator =(const ChangeLanguageRegistration_request&);
  
    bool checkIntegrity(void);

    const Language_v2* get_language(void) const;
    const Language_v2* get_hmiDisplayLanguage(void) const;

    void reset_language(void);
    bool set_language(const Language_v2& language_);
    void reset_hmiDisplayLanguage(void);
    bool set_hmiDisplayLanguage(const Language_v2& hmiDisplayLanguage_);

  private:
  
    friend class ChangeLanguageRegistration_requestMarshaller;


///  Requested SYNC voice engine (VR+TTS) language registration
      Language_v2* language;

///  Request display language registration
      Language_v2* hmiDisplayLanguage;
  };

}

#endif
