#ifndef NSRPC2COMMUNICATION_UI_GETCAPABILITIESRESPONSE_INCLUDE
#define NSRPC2COMMUNICATION_UI_GETCAPABILITIESRESPONSE_INCLUDE

#include <vector>
#include "JSONHandler/RPC2Response.h"

#include "../include/JSONHandler/ALRPCObjects/DisplayCapabilities.h"
#include "../include/JSONHandler/ALRPCObjects/HmiZoneCapabilities.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Mon Nov 19 10:37:39 2012
  source stamp	Mon Nov 19 10:17:20 2012
  author	robok0der
*/

namespace NsRPC2Communication
{
  namespace UI
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
      const NsAppLinkRPC::DisplayCapabilities& get_displayCapabilities(void);

      const std::vector< NsAppLinkRPC::HmiZoneCapabilities>& get_hmiZoneCapabilities(void);


// setters
      bool set_displayCapabilities(const NsAppLinkRPC::DisplayCapabilities& displayCapabilities);

/// 1 <= size <= 100
      bool set_hmiZoneCapabilities(const std::vector< NsAppLinkRPC::HmiZoneCapabilities>& hmiZoneCapabilities);


    private:

      friend class GetCapabilitiesResponseMarshaller;

      NsAppLinkRPC::DisplayCapabilities displayCapabilities;
      std::vector< NsAppLinkRPC::HmiZoneCapabilities> hmiZoneCapabilities;

    };
  }
}

#endif
