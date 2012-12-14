#ifndef NSRPC2COMMUNICATION_VR_GETLANGUAGERESPONSEMARSHALLER_INCLUDE
#define NSRPC2COMMUNICATION_VR_GETLANGUAGERESPONSEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/VR/GetLanguageResponse.h"

namespace NsRPC2Communication
{
  namespace VR
  {

    struct GetLanguageResponseMarshaller
    {
      static bool checkIntegrity(GetLanguageResponse& e);
      static bool checkIntegrityConst(const GetLanguageResponse& e);
    
      static bool fromString(const std::string& s,GetLanguageResponse& e);
      static const std::string toString(const GetLanguageResponse& e);
    
      static bool fromJSON(const Json::Value& s,GetLanguageResponse& e);
      static Json::Value toJSON(const GetLanguageResponse& e);
    };
  }
}

#endif
