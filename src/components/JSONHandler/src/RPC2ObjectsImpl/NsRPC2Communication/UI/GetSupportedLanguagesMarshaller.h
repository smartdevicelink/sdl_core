#ifndef NSRPC2COMMUNICATION_UI_GETSUPPORTEDLANGUAGEMARSHALLER
#define NSRPC2COMMUNICATION_UI_GETSUPPORTEDLANGUAGEMARSHALLER

#include <string>
#include <json/json.h>

#include "JSONHandler/RPC2Objects/NsRPC2Communication/UI/GetSupportedLanguages.h"

namespace NsRPC2Communication
{
  namespace UI
  {

    struct GetSupportedLanguagesMarshaller
    {
      static bool checkIntegrity(GetSupportedLanguages& e);
      static bool checkIntegrityConst(const GetSupportedLanguages& e);
    
      static bool fromString(const std::string& s,GetSupportedLanguages& e);
      static const std::string toString(const GetSupportedLanguages& e);
    
      static bool fromJSON(const Json::Value& s,GetSupportedLanguages& e);
      static Json::Value toJSON(const GetSupportedLanguages& e);
    };
  }
}

#endif