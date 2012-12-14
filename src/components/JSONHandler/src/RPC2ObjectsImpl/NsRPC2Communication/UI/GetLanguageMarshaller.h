#ifndef NSRPC2COMMUNICATION_UI_GETLANGUAGEMARSHALLER_INCLUDE
#define NSRPC2COMMUNICATION_UI_GETLANGUAGEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/GetLanguage.h"

namespace NsRPC2Communication
{
  namespace UI
  {

    struct GetLanguageMarshaller
    {
      static bool checkIntegrity(GetLanguage& e);
      static bool checkIntegrityConst(const GetLanguage& e);
    
      static bool fromString(const std::string& s,GetLanguage& e);
      static const std::string toString(const GetLanguage& e);
    
      static bool fromJSON(const Json::Value& s,GetLanguage& e);
      static Json::Value toJSON(const GetLanguage& e);
    };
  }
}

#endif
