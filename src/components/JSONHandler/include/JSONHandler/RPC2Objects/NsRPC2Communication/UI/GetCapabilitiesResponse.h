#ifndef NSRPC2COMMUNICATION_UI_GETCAPABILITIESRESPONSE_INCLUDE
#define NSRPC2COMMUNICATION_UI_GETCAPABILITIESRESPONSE_INCLUDE

#include <vector>
#include "JSONHandler/RPC2Response.h"

#include "../include/JSONHandler/ALRPCObjects/V1/DisplayCapabilities.h"
#include "../include/JSONHandler/ALRPCObjects/V1/HmiZoneCapabilities.h"
#include "../include/JSONHandler/ALRPCObjects/V1/SoftButtonCapabilities.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Tue Dec  4 15:06:30 2012
  source stamp	Tue Dec  4 14:21:32 2012
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

      const std::vector< NsAppLinkRPC::SoftButtonCapabilities>* get_softButtonCapabilities(void);

// setters
      bool set_displayCapabilities(const NsAppLinkRPC::DisplayCapabilities& displayCapabilities);

/// 1 <= size <= 100
      bool set_hmiZoneCapabilities(const std::vector< NsAppLinkRPC::HmiZoneCapabilities>& hmiZoneCapabilities);

/// 1 <= size <= 100
      bool set_softButtonCapabilities(const std::vector< NsAppLinkRPC::SoftButtonCapabilities>& softButtonCapabilities);

      void reset_softButtonCapabilities(void);


    private:

      friend class GetCapabilitiesResponseMarshaller;

      NsAppLinkRPC::DisplayCapabilities displayCapabilities;
      std::vector< NsAppLinkRPC::HmiZoneCapabilities> hmiZoneCapabilities;
      std::vector< NsAppLinkRPC::SoftButtonCapabilities>* softButtonCapabilities;

    };
  }
}

#endif
