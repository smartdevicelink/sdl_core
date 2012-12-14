#ifndef NSRPC2COMMUNICATION_VR_ONLANGUAGECHANGE_INCLUDE
#define NSRPC2COMMUNICATION_VR_ONLANGUAGECHANGE_INCLUDE

#include "JSONHandler/RPC2Notification.h"

#include "../include/JSONHandler/ALRPCObjects/V1/Language.h"

/*
  interface	NsRPC2Communication::VR
  version	1.2
  generated at	Fri Dec 14 06:14:25 2012
  source stamp	Fri Dec 14 06:14:23 2012
  author	robok0der
*/

namespace NsRPC2Communication
{
  namespace VR
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
      const NsAppLinkRPC::Language& get_language(void);

      int get_appId(void);


// setters
      bool set_language(const NsAppLinkRPC::Language& language);

      bool set_appId(int appId);


    private:

      friend class OnLanguageChangeMarshaller;

      NsAppLinkRPC::Language language;
      int appId;

    };
  }
}

#endif
