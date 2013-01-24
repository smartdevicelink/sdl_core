#ifndef NSRPC2COMMUNICATION_UI_PERFORMAUDIOPASSTHRUMARSHALLER_INCLUDE
#define NSRPC2COMMUNICATION_UI_PERFORMAUDIOPASSTHRUMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/PerformAudioPassThru.h"

namespace NsRPC2Communication
{
  namespace UI
  {

    struct PerformAudioPassThruMarshaller
    {
      static bool checkIntegrity(PerformAudioPassThru& e);
      static bool checkIntegrityConst(const PerformAudioPassThru& e);
    
      static bool fromString(const std::string& s,PerformAudioPassThru& e);
      static const std::string toString(const PerformAudioPassThru& e);
    
      static bool fromJSON(const Json::Value& s,PerformAudioPassThru& e);
      static Json::Value toJSON(const PerformAudioPassThru& e);
    };
  }
}

#endif
