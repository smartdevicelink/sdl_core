//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSRPC2COMMUNICATION_UI_PERFORMINTERACTIONRESPONSEMARSHALLER_INCLUDE
#define NSRPC2COMMUNICATION_UI_PERFORMINTERACTIONRESPONSEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/PerformInteractionResponse.h"

namespace NsRPC2Communication
{
  namespace UI
  {

    struct PerformInteractionResponseMarshaller
    {
      static bool checkIntegrity(PerformInteractionResponse& e);
      static bool checkIntegrityConst(const PerformInteractionResponse& e);
    
      static bool fromString(const std::string& s,PerformInteractionResponse& e);
      static const std::string toString(const PerformInteractionResponse& e);
    
      static bool fromJSON(const Json::Value& s,PerformInteractionResponse& e);
      static Json::Value toJSON(const PerformInteractionResponse& e);
    };
  }
}

#endif
