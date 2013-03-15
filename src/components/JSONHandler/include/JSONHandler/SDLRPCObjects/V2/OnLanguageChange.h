//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSSMARTDEVICELINKKRPCV2_ONLANGUAGECHANGE_INCLUDE
#define NSSMARTDEVICELINKKRPCV2_ONLANGUAGECHANGE_INCLUDE


#include "Language.h"
#include "JSONHandler/SDLRPCMessage.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	RC
*/

namespace NsSmartDeviceLinkRPCV2
{

  class OnLanguageChange : public NsSmartDeviceLinkRPC::SDLRPCMessage
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
