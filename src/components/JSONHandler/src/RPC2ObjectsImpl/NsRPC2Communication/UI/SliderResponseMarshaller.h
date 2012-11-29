#ifndef NSRPC2COMMUNICATION_UI_SLIDERRESPONSEMARSHALLER_INCLUDE
#define NSRPC2COMMUNICATION_UI_SLIDERRESPONSEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/SliderResponse.h"

namespace NsRPC2Communication
{
  namespace UI
  {

    struct SliderResponseMarshaller
    {
      static bool checkIntegrity(SliderResponse& e);
      static bool checkIntegrityConst(const SliderResponse& e);
    
      static bool fromString(const std::string& s,SliderResponse& e);
      static const std::string toString(const SliderResponse& e);
    
      static bool fromJSON(const Json::Value& s,SliderResponse& e);
      static Json::Value toJSON(const SliderResponse& e);
    };
  }
}

#endif
