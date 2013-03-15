//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSRPC2COMMUNICATION_VR_GETLANGUAGERESPONSE_INCLUDE
#define NSRPC2COMMUNICATION_VR_GETLANGUAGERESPONSE_INCLUDE

#include "JSONHandler/RPC2Response.h"

#include "../include/JSONHandler/SDLRPCObjects/V2/Language.h"

/*
  interface	NsRPC2Communication::VR
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	RC
*/

namespace NsRPC2Communication
{
  namespace VR
  {

    class GetLanguageResponse : public ::NsRPC2Communication::RPC2Response
    {
    public:
    
      GetLanguageResponse(const GetLanguageResponse& c);
      GetLanguageResponse(void);
    
      GetLanguageResponse& operator =(const GetLanguageResponse&);
    
      virtual ~GetLanguageResponse(void);
    
      bool checkIntegrity(void);
    
// getters
      const NsSmartDeviceLinkRPCV2::Language& get_language(void);


// setters
      bool set_language(const NsSmartDeviceLinkRPCV2::Language& language);


    private:

      friend class GetLanguageResponseMarshaller;

      NsSmartDeviceLinkRPCV2::Language language;

    };
  }
}

#endif
