#ifndef NSRPC2COMMUNICATION_BUTTONS_GETCAPABILITIESRESPONSE_INCLUDE
#define NSRPC2COMMUNICATION_BUTTONS_GETCAPABILITIESRESPONSE_INCLUDE

#include <vector>
#include "JSONHandler/RPC2Response.h"

#include "../include/JSONHandler/ALRPCObjects/ButtonCapabilities.h"

/*
  interface	NsRPC2Communication::Buttons
  version	1.2
  generated at	Mon Nov 19 08:20:05 2012
  source stamp	Mon Nov 19 07:04:40 2012
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


// setters
/// 1 <= size <= 100
      bool set_capabilities(const std::vector< NsAppLinkRPC::ButtonCapabilities>& capabilities);


    private:

      friend class GetCapabilitiesResponseMarshaller;

      std::vector< NsAppLinkRPC::ButtonCapabilities> capabilities;

    };
  }
}

#endif
