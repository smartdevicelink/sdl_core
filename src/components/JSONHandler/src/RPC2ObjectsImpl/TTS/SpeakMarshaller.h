#ifndef RPC2COMMUNICATION_TTS_SPEAKMARSHALLER_INCLUDE
#define RPC2COMMUNICATION_TTS_SPEAKMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/RPC2Objects/TTS/Speak.h"

namespace RPC2Communication
{
  namespace TTS
  {

    struct SpeakMarshaller
    {
      static bool checkIntegrity(Speak& e);
      static bool checkIntegrityConst(const Speak& e);
    
      static bool fromString(const std::string& s,Speak& e);
      static const std::string toString(const Speak& e);
    
      static bool fromJSON(const Json::Value& s,Speak& e);
      static Json::Value toJSON(const Speak& e);
    };
  }
}

#endif
