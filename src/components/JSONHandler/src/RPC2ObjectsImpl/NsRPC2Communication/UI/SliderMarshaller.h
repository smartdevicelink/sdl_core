#ifndef NSRPC2COMMUNICATION_UI_SLIDERMARSHALLER_INCLUDE
#define NSRPC2COMMUNICATION_UI_SLIDERMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/Slider.h"

namespace NsRPC2Communication
{
  namespace UI
  {

    struct SliderMarshaller
    {
      static bool checkIntegrity(Slider& e);
      static bool checkIntegrityConst(const Slider& e);
    
      static bool fromString(const std::string& s,Slider& e);
      static const std::string toString(const Slider& e);
    
      static bool fromJSON(const Json::Value& s,Slider& e);
      static Json::Value toJSON(const Slider& e);
    };
  }
}

#endif
