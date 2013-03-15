//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSRPC2COMMUNICATION_UI_ALERTRESPONSEMARSHALLER_INCLUDE
#define NSRPC2COMMUNICATION_UI_ALERTRESPONSEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/AlertResponse.h"

namespace NsRPC2Communication
{
  namespace UI
  {

    struct AlertResponseMarshaller
    {
      static bool checkIntegrity(AlertResponse& e);
      static bool checkIntegrityConst(const AlertResponse& e);
    
      static bool fromString(const std::string& s,AlertResponse& e);
      static const std::string toString(const AlertResponse& e);
    
      static bool fromJSON(const Json::Value& s,AlertResponse& e);
      static Json::Value toJSON(const AlertResponse& e);
    };
  }
}

#endif
