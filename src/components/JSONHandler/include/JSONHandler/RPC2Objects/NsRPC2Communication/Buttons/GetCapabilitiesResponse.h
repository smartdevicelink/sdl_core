#ifndef NSRPC2COMMUNICATION_BUTTONS_GETCAPABILITIESRESPONSE_INCLUDE
#define NSRPC2COMMUNICATION_BUTTONS_GETCAPABILITIESRESPONSE_INCLUDE

#include <vector>
#include "JSONHandler/RPC2Response.h"

#include "../include/JSONHandler/ALRPCObjects/V1/ButtonCapabilities.h"
#include "../include/JSONHandler/ALRPCObjects/V1/PresetBankCapabilities.h"

/*
  interface	NsRPC2Communication::Buttons
  version	1.2
  generated at	Tue Dec  4 15:06:30 2012
  source stamp	Tue Dec  4 14:21:32 2012
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
      const std::vector< NsAppLinkRPC::ButtonCapabilities>& get_capabilities(void);

      const NsAppLinkRPC::PresetBankCapabilities* get_presetBankCapabilities(void);

// setters
/// 1 <= size <= 100
      bool set_capabilities(const std::vector< NsAppLinkRPC::ButtonCapabilities>& capabilities);

      bool set_presetBankCapabilities(const NsAppLinkRPC::PresetBankCapabilities& presetBankCapabilities);

      void reset_presetBankCapabilities(void);


    private:

      friend class GetCapabilitiesResponseMarshaller;

      std::vector< NsAppLinkRPC::ButtonCapabilities> capabilities;
      NsAppLinkRPC::PresetBankCapabilities* presetBankCapabilities;

    };
  }
}

#endif
