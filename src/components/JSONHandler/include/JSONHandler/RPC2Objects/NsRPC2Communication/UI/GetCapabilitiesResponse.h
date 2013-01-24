#ifndef NSRPC2COMMUNICATION_UI_GETCAPABILITIESRESPONSE_INCLUDE
#define NSRPC2COMMUNICATION_UI_GETCAPABILITIESRESPONSE_INCLUDE

#include <vector>
#include "JSONHandler/RPC2Response.h"

#include "../include/JSONHandler/ALRPCObjects/V2/DisplayCapabilities.h"
#include "../include/JSONHandler/ALRPCObjects/V2/HmiZoneCapabilities.h"
#include "../include/JSONHandler/ALRPCObjects/V2/SoftButtonCapabilities.h"

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

    class GetCapabilitiesResponse : public ::NsRPC2Communication::RPC2Response
    {
    public:
    
      GetCapabilitiesResponse(const GetCapabilitiesResponse& c);
      GetCapabilitiesResponse(void);
    
      GetCapabilitiesResponse& operator =(const GetCapabilitiesResponse&);
    
      virtual ~GetCapabilitiesResponse(void);
    
      bool checkIntegrity(void);
    
// getters
      const NsAppLinkRPCV2::DisplayCapabilities& get_displayCapabilities(void);

      const std::vector< NsAppLinkRPCV2::HmiZoneCapabilities>& get_hmiZoneCapabilities(void);

      const std::vector< NsAppLinkRPCV2::SoftButtonCapabilities>* get_softButtonCapabilities(void);

// setters
      bool set_displayCapabilities(const NsAppLinkRPCV2::DisplayCapabilities& displayCapabilities);

/// 1 <= size <= 100
      bool set_hmiZoneCapabilities(const std::vector< NsAppLinkRPCV2::HmiZoneCapabilities>& hmiZoneCapabilities);

/// 1 <= size <= 100
      bool set_softButtonCapabilities(const std::vector< NsAppLinkRPCV2::SoftButtonCapabilities>& softButtonCapabilities);

      void reset_softButtonCapabilities(void);


    private:

      friend class GetCapabilitiesResponseMarshaller;

      NsAppLinkRPCV2::DisplayCapabilities displayCapabilities;
      std::vector< NsAppLinkRPCV2::HmiZoneCapabilities> hmiZoneCapabilities;
      std::vector< NsAppLinkRPCV2::SoftButtonCapabilities>* softButtonCapabilities;

    };
  }
}

#endif
