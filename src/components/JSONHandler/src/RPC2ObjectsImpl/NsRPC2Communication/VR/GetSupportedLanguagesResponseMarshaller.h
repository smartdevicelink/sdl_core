//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSRPC2COMMUNICATION_VR_GETSUPPORTEDLANGUAGESRESPONSEMARSHALLER_INCLUDE
#define NSRPC2COMMUNICATION_VR_GETSUPPORTEDLANGUAGESRESPONSEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/VR/GetSupportedLanguagesResponse.h"

namespace NsRPC2Communication
{
  namespace VR
  {

    struct GetSupportedLanguagesResponseMarshaller
    {
      static bool checkIntegrity(GetSupportedLanguagesResponse& e);
      static bool checkIntegrityConst(const GetSupportedLanguagesResponse& e);
    
      static bool fromString(const std::string& s,GetSupportedLanguagesResponse& e);
      static const std::string toString(const GetSupportedLanguagesResponse& e);
    
      static bool fromJSON(const Json::Value& s,GetSupportedLanguagesResponse& e);
      static Json::Value toJSON(const GetSupportedLanguagesResponse& e);
    };
  }
}

#endif
