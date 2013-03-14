#ifndef NSRPC2COMMUNICATION_VR_ONLANGUAGECHANGE_INCLUDE
#define NSRPC2COMMUNICATION_VR_ONLANGUAGECHANGE_INCLUDE

#include "JSONHandler/RPC2Notification.h"

#include "../include/JSONHandler/SDLRPCObjects/V2/Language.h"

/*
  interface	NsRPC2Communication::VR
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
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
      const NsSmartDeviceLinkRPCV2::Language& get_language(void);


// setters
      bool set_language(const NsSmartDeviceLinkRPCV2::Language& language);


    private:

      friend class OnLanguageChangeMarshaller;

      NsSmartDeviceLinkRPCV2::Language language;

    };
  }
}

#endif
