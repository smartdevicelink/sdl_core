//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSRPC2COMMUNICATION_UI_PERFORMINTERACTIONMARSHALLER_INCLUDE
#define NSRPC2COMMUNICATION_UI_PERFORMINTERACTIONMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/PerformInteraction.h"

namespace NsRPC2Communication
{
  namespace UI
  {

    struct PerformInteractionMarshaller
    {
      static bool checkIntegrity(PerformInteraction& e);
      static bool checkIntegrityConst(const PerformInteraction& e);
    
      static bool fromString(const std::string& s,PerformInteraction& e);
      static const std::string toString(const PerformInteraction& e);
    
      static bool fromJSON(const Json::Value& s,PerformInteraction& e);
      static Json::Value toJSON(const PerformInteraction& e);
    };
  }
}

#endif
