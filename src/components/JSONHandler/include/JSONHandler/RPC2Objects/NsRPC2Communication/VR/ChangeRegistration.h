#ifndef NSRPC2COMMUNICATION_VR_CHANGEREGISTRATION_INCLUDE
#define NSRPC2COMMUNICATION_VR_CHANGEREGISTRATION_INCLUDE

#include "JSONHandler/RPC2Request.h"

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

    class ChangeRegistration : public ::NsRPC2Communication::RPC2Request
    {
    public:
    
      ChangeRegistration(const ChangeRegistration& c);
      ChangeRegistration(void);
    
      ChangeRegistration& operator =(const ChangeRegistration&);
    
      virtual ~ChangeRegistration(void);
    
      bool checkIntegrity(void);
    
// getters
      const NsAppLinkRPC::Language& get_language(void);

      int get_appId(void);


// setters
      bool set_language(const NsAppLinkRPC::Language& language);

      bool set_appId(int appId);


    private:

      friend class ChangeRegistrationMarshaller;

      NsAppLinkRPC::Language language;
      int appId;

    };
  }
}

#endif
