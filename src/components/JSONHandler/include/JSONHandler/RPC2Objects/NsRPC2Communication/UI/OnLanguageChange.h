//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSRPC2COMMUNICATION_UI_ONLANGUAGECHANGE_INCLUDE
#define NSRPC2COMMUNICATION_UI_ONLANGUAGECHANGE_INCLUDE

#include "JSONHandler/RPC2Notification.h"

#include "../include/JSONHandler/SDLRPCObjects/V2/Language.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	RC
*/

namespace NsRPC2Communication
{
  namespace UI
  {

    class OnLanguageChange : public ::NsRPC2Communication::RPC2Notification
    {
    public:
    
      OnLanguageChange(const OnLanguageChange& c);
      OnLanguageChange(void);
    
      OnLanguageChange& operator =(const OnLanguageChange&);
    
      virtual ~OnLanguageChange(void);
    
      bool checkIntegrity(void);
    
// getters
      const NsSmartDeviceLinkRPCV2::Language& get_hmiDisplayLanguage(void);


// setters
      bool set_hmiDisplayLanguage(const NsSmartDeviceLinkRPCV2::Language& hmiDisplayLanguage);


    private:

      friend class OnLanguageChangeMarshaller;

      NsSmartDeviceLinkRPCV2::Language hmiDisplayLanguage;

    };
  }
}

#endif
