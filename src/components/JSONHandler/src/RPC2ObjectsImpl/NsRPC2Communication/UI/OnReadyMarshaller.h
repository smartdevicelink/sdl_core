//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSRPC2COMMUNICATION_UI_ONREADYMARSHALLER_INCLUDE
#define NSRPC2COMMUNICATION_UI_ONREADYMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/OnReady.h"

namespace NsRPC2Communication
{
  namespace UI
  {

    struct OnReadyMarshaller
    {
      static bool checkIntegrity(OnReady& e);
      static bool checkIntegrityConst(const OnReady& e);
    
      static bool fromString(const std::string& s,OnReady& e);
      static const std::string toString(const OnReady& e);
    
      static bool fromJSON(const Json::Value& s,OnReady& e);
      static Json::Value toJSON(const OnReady& e);
    };
  }
}

#endif
