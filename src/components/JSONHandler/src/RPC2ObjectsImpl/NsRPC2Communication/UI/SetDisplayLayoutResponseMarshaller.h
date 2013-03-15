//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSRPC2COMMUNICATION_UI_SETDISPLAYLAYOUTRESPONSEMARSHALLER_INCLUDE
#define NSRPC2COMMUNICATION_UI_SETDISPLAYLAYOUTRESPONSEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/SetDisplayLayoutResponse.h"

namespace NsRPC2Communication
{
  namespace UI
  {

    struct SetDisplayLayoutResponseMarshaller
    {
      static bool checkIntegrity(SetDisplayLayoutResponse& e);
      static bool checkIntegrityConst(const SetDisplayLayoutResponse& e);
    
      static bool fromString(const std::string& s,SetDisplayLayoutResponse& e);
      static const std::string toString(const SetDisplayLayoutResponse& e);
    
      static bool fromJSON(const Json::Value& s,SetDisplayLayoutResponse& e);
      static Json::Value toJSON(const SetDisplayLayoutResponse& e);
    };
  }
}

#endif
