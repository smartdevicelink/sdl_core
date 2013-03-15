//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSRPC2COMMUNICATION_UI_PERFORMAUDIOPASSTHRURESPONSEMARSHALLER_INCLUDE
#define NSRPC2COMMUNICATION_UI_PERFORMAUDIOPASSTHRURESPONSEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/PerformAudioPassThruResponse.h"

namespace NsRPC2Communication
{
  namespace UI
  {

    struct PerformAudioPassThruResponseMarshaller
    {
      static bool checkIntegrity(PerformAudioPassThruResponse& e);
      static bool checkIntegrityConst(const PerformAudioPassThruResponse& e);
    
      static bool fromString(const std::string& s,PerformAudioPassThruResponse& e);
      static const std::string toString(const PerformAudioPassThruResponse& e);
    
      static bool fromJSON(const Json::Value& s,PerformAudioPassThruResponse& e);
      static Json::Value toJSON(const PerformAudioPassThruResponse& e);
    };
  }
}

#endif
