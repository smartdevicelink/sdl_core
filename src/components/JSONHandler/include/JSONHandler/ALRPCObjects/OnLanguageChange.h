#ifndef NSAPPLINKRPC_ONLANGUAGECHANGE_INCLUDE
#define NSAPPLINKRPC_ONLANGUAGECHANGE_INCLUDE


#include "Language_v2.h"
#include "JSONHandler/ALRPCNotification.h"


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

  class OnLanguageChange : public ALRPCNotification
  {
  public:
  
    OnLanguageChange(const OnLanguageChange& c);
    OnLanguageChange(void);
    
    virtual ~OnLanguageChange(void);
  
    bool checkIntegrity(void);

    const Language_v2& get_language(void) const;
    const Language_v2& get_hmiDisplayLanguage(void) const;

    bool set_language(const Language_v2& language_);
    bool set_hmiDisplayLanguage(const Language_v2& hmiDisplayLanguage_);

  private:
  
    friend class OnLanguageChangeMarshaller;


///  Current SYNC voice engine (VR+TTS) language
      Language_v2 language;

///  Current display language
      Language_v2 hmiDisplayLanguage;
  };

}

#endif
