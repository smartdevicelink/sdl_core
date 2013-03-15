//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSRPC2COMMUNICATION_UI_ENDAUDIOPASSTHRURESPONSEMARSHALLER_INCLUDE
#define NSRPC2COMMUNICATION_UI_ENDAUDIOPASSTHRURESPONSEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/EndAudioPassThruResponse.h"

namespace NsRPC2Communication
{
  namespace UI
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
