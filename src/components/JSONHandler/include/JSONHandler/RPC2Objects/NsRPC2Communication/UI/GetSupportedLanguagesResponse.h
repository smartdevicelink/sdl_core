#ifndef NSRPC2COMMUNICATION_UI_GETSUPPORTEDLANGUAGESRESPONSE_INCLUDE
#define NSRPC2COMMUNICATION_UI_GETSUPPORTEDLANGUAGESRESPONSE_INCLUDE

#include <vector>
#include "JSONHandler/RPC2Response.h"

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

    class GetSupportedLanguagesResponse : public ::NsRPC2Communication::RPC2Response
    {
    public:
    
      GetSupportedLanguagesResponse(const GetSupportedLanguagesResponse& c);
      GetSupportedLanguagesResponse(void);
    
      GetSupportedLanguagesResponse& operator =(const GetSupportedLanguagesResponse&);
    
      virtual ~GetSupportedLanguagesResponse(void);
    
      bool checkIntegrity(void);
    
// getters
      const std::vector< NsSmartDeviceLinkRPCV2::Language>& get_languages(void);


// setters
/// 1 <= size <= 100
      bool set_languages(const std::vector< NsSmartDeviceLinkRPCV2::Language>& languages);


    private:

      friend class GetSupportedLanguagesResponseMarshaller;

      std::vector< NsSmartDeviceLinkRPCV2::Language> languages;

    };
  }
}

#endif
