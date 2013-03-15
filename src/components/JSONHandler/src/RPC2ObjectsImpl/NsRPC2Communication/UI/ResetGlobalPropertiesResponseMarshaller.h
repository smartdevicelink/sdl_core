//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSRPC2COMMUNICATION_UI_RESETGLOBALPROPERTIESRESPONSEMARSHALLER_INCLUDE
#define NSRPC2COMMUNICATION_UI_RESETGLOBALPROPERTIESRESPONSEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/ResetGlobalPropertiesResponse.h"

namespace NsRPC2Communication
{
  namespace UI
  {

    struct ResetGlobalPropertiesResponseMarshaller
    {
      static bool checkIntegrity(ResetGlobalPropertiesResponse& e);
      static bool checkIntegrityConst(const ResetGlobalPropertiesResponse& e);
    
      static bool fromString(const std::string& s,ResetGlobalPropertiesResponse& e);
      static const std::string toString(const ResetGlobalPropertiesResponse& e);
    
      static bool fromJSON(const Json::Value& s,ResetGlobalPropertiesResponse& e);
      static Json::Value toJSON(const ResetGlobalPropertiesResponse& e);
    };
  }
}

#endif
