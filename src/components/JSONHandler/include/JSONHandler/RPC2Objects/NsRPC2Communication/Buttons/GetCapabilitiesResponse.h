#ifndef NSRPC2COMMUNICATION_BUTTONS_GETCAPABILITIESRESPONSE_INCLUDE
#define NSRPC2COMMUNICATION_BUTTONS_GETCAPABILITIESRESPONSE_INCLUDE

#include <vector>
#include "JSONHandler/RPC2Response.h"

#include "../include/JSONHandler/ALRPCObjects/V2/ButtonCapabilities.h"
#include "../include/JSONHandler/ALRPCObjects/V2/PresetBankCapabilities.h"

/*
  interface	NsRPC2Communication::Buttons
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	robok0der
*/

namespace NsRPC2Communication
{
  namespace Buttons
  {

    class GetCapabilitiesResponse : public ::NsRPC2Communication::RPC2Response
    {
    public:
    
      GetCapabilitiesResponse(const GetCapabilitiesResponse& c);
      GetCapabilitiesResponse(void);
    
      GetCapabilitiesResponse& operator =(const GetCapabilitiesResponse&);
    
      virtual ~GetCapabilitiesResponse(void);
    
      bool checkIntegrity(void);
    
// getters
      const std::vector< NsAppLinkRPCV2::ButtonCapabilities>& get_capabilities(void);

      const NsAppLinkRPCV2::PresetBankCapabilities* get_presetBankCapabilities(void);

// setters
/// 1 <= size <= 100
      bool set_capabilities(const std::vector< NsAppLinkRPCV2::ButtonCapabilities>& capabilities);

      bool set_presetBankCapabilities(const NsAppLinkRPCV2::PresetBankCapabilities& presetBankCapabilities);

      void reset_presetBankCapabilities(void);


    private:

      friend class GetCapabilitiesResponseMarshaller;

      std::vector< NsAppLinkRPCV2::ButtonCapabilities> capabilities;
      NsAppLinkRPCV2::PresetBankCapabilities* presetBankCapabilities;

    };
  }
}

#endif
