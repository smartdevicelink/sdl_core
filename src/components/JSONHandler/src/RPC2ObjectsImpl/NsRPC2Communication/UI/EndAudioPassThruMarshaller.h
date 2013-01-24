#ifndef NSRPC2COMMUNICATION_UI_ENDAUDIOPASSTHRUMARSHALLER_INCLUDE
#define NSRPC2COMMUNICATION_UI_ENDAUDIOPASSTHRUMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/EndAudioPassThru.h"

namespace NsRPC2Communication
{
  namespace UI
  {

    struct EndAudioPassThruMarshaller
    {
      static bool checkIntegrity(EndAudioPassThru& e);
      static bool checkIntegrityConst(const EndAudioPassThru& e);
    
      static bool fromString(const std::string& s,EndAudioPassThru& e);
      static const std::string toString(const EndAudioPassThru& e);
    
      static bool fromJSON(const Json::Value& s,EndAudioPassThru& e);
      static Json::Value toJSON(const EndAudioPassThru& e);
    };
  }
}

#endif
