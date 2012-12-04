#ifndef NSAPPLINKRPCV2_CHANGELANGUAGEREGISTRATION_REQUEST_INCLUDE
#define NSAPPLINKRPCV2_CHANGELANGUAGEREGISTRATION_REQUEST_INCLUDE


#include "Language.h"
#include "JSONHandler/ALRPC2Message.hh"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Tue Dec  4 14:30:13 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

namespace NsAppLinkRPCV2
{

  class ChangeLanguageRegistration_request : public ALRPC2Message
  {
  public:
  
    ChangeLanguageRegistration_request(const ChangeLanguageRegistration_request& c);
    ChangeLanguageRegistration_request(void);
    
    virtual ~ChangeLanguageRegistration_request(void);
  
    bool checkIntegrity(void);

    const Language& get_language(void) const;
    const Language& get_hmiDisplayLanguage(void) const;

    bool set_language(const Language& language_);
    bool set_hmiDisplayLanguage(const Language& hmiDisplayLanguage_);

  private:
  
    friend class ChangeLanguageRegistration_requestMarshaller;


///  Requested SYNC voice engine (VR+TTS) language registration
      Language language;

///  Request display language registration
      Language hmiDisplayLanguage;
  };

}

#endif
