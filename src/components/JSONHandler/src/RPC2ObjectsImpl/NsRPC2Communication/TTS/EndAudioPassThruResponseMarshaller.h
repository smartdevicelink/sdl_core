#ifndef NSRPC2COMMUNICATION_TTS_ENDAUDIOPASSTHRURESPONSEMARSHALLER_INCLUDE
#define NSRPC2COMMUNICATION_TTS_ENDAUDIOPASSTHRURESPONSEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/TTS/EndAudioPassThruResponse.h"

namespace NsRPC2Communication
{
  namespace TTS
  {

    struct EndAudioPassThruResponseMarshaller
    {
      static bool checkIntegrity(EndAudioPassThruResponse& e);
      static bool checkIntegrityConst(const EndAudioPassThruResponse& e);
    
      static bool fromString(const std::string& s,EndAudioPassThruResponse& e);
      static const std::string toString(const EndAudioPassThruResponse& e);
    
      static bool fromJSON(const Json::Value& s,EndAudioPassThruResponse& e);
      static Json::Value toJSON(const EndAudioPassThruResponse& e);
    };
  }
}

#endif
