#ifndef NSRPC2COMMUNICATION_TTS_GETCAPABILITIESRESPONSE_INCLUDE
#define NSRPC2COMMUNICATION_TTS_GETCAPABILITIESRESPONSE_INCLUDE

#include <vector>
#include "JSONHandler/RPC2Response.h"

#include "../include/JSONHandler/ALRPCObjects/V1/SpeechCapabilities.h"

/*
  interface	NsRPC2Communication::TTS
  version	1.2
  generated at	Fri Dec 14 06:14:25 2012
  source stamp	Fri Dec 14 06:14:23 2012
  author	robok0der
*/

namespace NsRPC2Communication
{
  namespace TTS
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
      const std::vector< NsAppLinkRPC::SpeechCapabilities>& get_capabilities(void);


// setters
/// 1 <= size <= 100
      bool set_capabilities(const std::vector< NsAppLinkRPC::SpeechCapabilities>& capabilities);


    private:

      friend class GetCapabilitiesResponseMarshaller;

      std::vector< NsAppLinkRPC::SpeechCapabilities> capabilities;

    };
  }
}

#endif
