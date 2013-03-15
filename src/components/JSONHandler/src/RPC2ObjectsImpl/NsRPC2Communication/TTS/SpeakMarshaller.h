//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSRPC2COMMUNICATION_TTS_SPEAKMARSHALLER_INCLUDE
#define NSRPC2COMMUNICATION_TTS_SPEAKMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/TTS/Speak.h"

namespace NsRPC2Communication
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
