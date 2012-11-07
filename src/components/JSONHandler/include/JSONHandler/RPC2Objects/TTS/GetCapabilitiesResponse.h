#ifndef RPC2COMMUNICATION_TTS_GETCAPABILITIESRESPONSE_INCLUDE
#define RPC2COMMUNICATION_TTS_GETCAPABILITIESRESPONSE_INCLUDE

#include <vector>
#include "JSONHandler/RPC2Response.h"

#include "../include/JSONHandler/ALRPCObjects/SpeechCapabilities.h"

/*
  interface	RPC2Communication::TTS
  version	1.2
  generated at	Wed Nov  7 11:25:55 2012
  source stamp	Wed Nov  7 09:34:03 2012
  author	robok0der
*/

namespace RPC2Communication
{
  namespace TTS
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
      const std::vector< ::AppLinkRPC::SpeechCapabilities>& get_capabilities(void);


// setters
/// 1 <= size <= 100
      bool set_capabilities(const std::vector< ::AppLinkRPC::SpeechCapabilities>& capabilities);


    private:

      friend class GetCapabilitiesResponseMarshaller;

      std::vector< ::AppLinkRPC::SpeechCapabilities> capabilities;

    };
  }
}

#endif
