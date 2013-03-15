//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSRPC2COMMUNICATION_UI_SHOWMARSHALLER_INCLUDE
#define NSRPC2COMMUNICATION_UI_SHOWMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/Show.h"

namespace NsRPC2Communication
{
  namespace UI
  {

    struct ShowMarshaller
    {
      static bool checkIntegrity(Show& e);
      static bool checkIntegrityConst(const Show& e);
    
      static bool fromString(const std::string& s,Show& e);
      static const std::string toString(const Show& e);
    
      static bool fromJSON(const Json::Value& s,Show& e);
      static Json::Value toJSON(const Show& e);
    };
  }
}

#endif
