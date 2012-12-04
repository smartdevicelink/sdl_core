#ifndef NSAPPLINKRPCV2_ONLANGUAGECHANGE_INCLUDE
#define NSAPPLINKRPCV2_ONLANGUAGECHANGE_INCLUDE


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

  class OnLanguageChange : public ALRPC2Message
  {
  public:
  
    OnLanguageChange(const OnLanguageChange& c);
    OnLanguageChange(void);
    
    virtual ~OnLanguageChange(void);
  
    bool checkIntegrity(void);

    const Language& get_language(void) const;
    const Language& get_hmiDisplayLanguage(void) const;

    bool set_language(const Language& language_);
    bool set_hmiDisplayLanguage(const Language& hmiDisplayLanguage_);

  private:
  
    friend class OnLanguageChangeMarshaller;


///  Current SYNC voice engine (VR+TTS) language
      Language language;

///  Current display language
      Language hmiDisplayLanguage;
  };

}

#endif
