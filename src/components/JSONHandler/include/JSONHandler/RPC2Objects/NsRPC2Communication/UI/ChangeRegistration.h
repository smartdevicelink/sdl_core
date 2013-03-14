#ifndef NSRPC2COMMUNICATION_UI_CHANGEREGISTRATION_INCLUDE
#define NSRPC2COMMUNICATION_UI_CHANGEREGISTRATION_INCLUDE

#include "JSONHandler/RPC2Request.h"

#include "../include/JSONHandler/SDLRPCObjects/V2/Language.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	robok0der
*/

namespace NsRPC2Communication
{
  namespace UI
  {

    class ChangeRegistration : public ::NsRPC2Communication::RPC2Request
    {
    public:
    
      ChangeRegistration(const ChangeRegistration& c);
      ChangeRegistration(void);
    
      ChangeRegistration& operator =(const ChangeRegistration&);
    
      virtual ~ChangeRegistration(void);
    
      bool checkIntegrity(void);
    
// getters
      const NsSmartDeviceLinkRPCV2::Language& get_hmiDisplayLanguage(void);

      int get_appId(void);


// setters
      bool set_hmiDisplayLanguage(const NsSmartDeviceLinkRPCV2::Language& hmiDisplayLanguage);

      bool set_appId(int appId);


    private:

      friend class ChangeRegistrationMarshaller;

      NsSmartDeviceLinkRPCV2::Language hmiDisplayLanguage;
      int appId;

    };
  }
}

#endif
