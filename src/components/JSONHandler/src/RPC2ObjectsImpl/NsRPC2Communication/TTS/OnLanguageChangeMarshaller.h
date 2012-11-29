#ifndef NSRPC2COMMUNICATION_TTS_ONLANGUAGECHANGEMARSHALLER_INCLUDE
#define NSRPC2COMMUNICATION_TTS_ONLANGUAGECHANGEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/TTS/OnLanguageChange.h"

namespace NsRPC2Communication
{
  namespace TTS
  {

    struct OnLanguageChangeMarshaller
    {
      static bool checkIntegrity(OnLanguageChange& e);
      static bool checkIntegrityConst(const OnLanguageChange& e);
    
      static bool fromString(const std::string& s,OnLanguageChange& e);
      static const std::string toString(const OnLanguageChange& e);
    
      static bool fromJSON(const Json::Value& s,OnLanguageChange& e);
      static Json::Value toJSON(const OnLanguageChange& e);
    };
  }
}

#endif
