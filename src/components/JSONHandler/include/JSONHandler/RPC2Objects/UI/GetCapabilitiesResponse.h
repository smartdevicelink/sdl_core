#ifndef RPC2COMMUNICATION_UI_GETCAPABILITIESRESPONSE_INCLUDE
#define RPC2COMMUNICATION_UI_GETCAPABILITIESRESPONSE_INCLUDE

#include <vector>
#include "JSONHandler/RPC2Response.h"

#include "../include/JSONHandler/ALRPCObjects/DisplayCapabilities.h"
#include "../include/JSONHandler/ALRPCObjects/HmiZoneCapabilities.h"

/*
  interface	RPC2Communication::UI
  version	1.2
  generated at	Wed Nov  7 11:26:14 2012
  source stamp	Wed Nov  7 09:29:07 2012
  author	robok0der
*/

namespace RPC2Communication
{
  namespace UI
  {

    class GetCapabilitiesResponse : public ::RPC2Communication::RPC2Response
    {
    public:
    
      GetCapabilitiesResponse(const GetCapabilitiesResponse& c);
      GetCapabilitiesResponse(void);
    
      GetCapabilitiesResponse& operator =(const GetCapabilitiesResponse&);
    
      virtual ~GetCapabilitiesResponse(void);
    
      bool checkIntegrity(void);
    
// getters
      const ::AppLinkRPC::DisplayCapabilities& get_displayCapabilities(void);

      const std::vector< ::AppLinkRPC::HmiZoneCapabilities>& get_hmiZoneCapabilities(void);


// setters
      bool set_displayCapabilities(const ::AppLinkRPC::DisplayCapabilities& displayCapabilities);

/// 1 <= size <= 100
      bool set_hmiZoneCapabilities(const std::vector< ::AppLinkRPC::HmiZoneCapabilities>& hmiZoneCapabilities);


    private:

      friend class GetCapabilitiesResponseMarshaller;

      ::AppLinkRPC::DisplayCapabilities displayCapabilities;
      std::vector< ::AppLinkRPC::HmiZoneCapabilities> hmiZoneCapabilities;

    };
  }
}

#endif
