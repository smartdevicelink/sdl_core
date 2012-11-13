#ifndef RPC2COMMUNICATION_TTS_SPEAKRESPONSEMARSHALLER_INCLUDE
#define RPC2COMMUNICATION_TTS_SPEAKRESPONSEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/RPC2Objects/TTS/SpeakResponse.h"

namespace RPC2Communication
{
  namespace TTS
  {

    struct SpeakResponseMarshaller
    {
      static bool checkIntegrity(SpeakResponse& e);
      static bool checkIntegrityConst(const SpeakResponse& e);
    
      static bool fromString(const std::string& s,SpeakResponse& e);
      static const std::string toString(const SpeakResponse& e);
    
      static bool fromJSON(const Json::Value& s,SpeakResponse& e);
      static Json::Value toJSON(const SpeakResponse& e);
    };
  }
}

#endif
