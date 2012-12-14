#ifndef NSRPC2COMMUNICATION_TTS_ONAUDIOPASSTHRUMARSHALLER_INCLUDE
#define NSRPC2COMMUNICATION_TTS_ONAUDIOPASSTHRUMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/TTS/OnAudioPassThru.h"

namespace NsRPC2Communication
{
  namespace TTS
  {

    struct OnAudioPassThruMarshaller
    {
      static bool checkIntegrity(OnAudioPassThru& e);
      static bool checkIntegrityConst(const OnAudioPassThru& e);
    
      static bool fromString(const std::string& s,OnAudioPassThru& e);
      static const std::string toString(const OnAudioPassThru& e);
    
      static bool fromJSON(const Json::Value& s,OnAudioPassThru& e);
      static Json::Value toJSON(const OnAudioPassThru& e);
    };
  }
}

#endif
