//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSRPC2COMMUNICATION_TTS_CHANGEREGISTRATION_INCLUDE
#define NSRPC2COMMUNICATION_TTS_CHANGEREGISTRATION_INCLUDE

#include "JSONHandler/RPC2Request.h"

#include "../include/JSONHandler/SDLRPCObjects/V2/Language.h"

/*
  interface	NsRPC2Communication::TTS
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
*/

namespace NsRPC2Communication
{
  namespace TTS
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
      const NsSmartDeviceLinkRPCV2::Language& get_language(void);

      int get_appId(void);


// setters
      bool set_language(const NsSmartDeviceLinkRPCV2::Language& language);

      bool set_appId(int appId);


    private:

      friend class ChangeRegistrationMarshaller;

      NsSmartDeviceLinkRPCV2::Language language;
      int appId;

    };
  }
}

#endif
