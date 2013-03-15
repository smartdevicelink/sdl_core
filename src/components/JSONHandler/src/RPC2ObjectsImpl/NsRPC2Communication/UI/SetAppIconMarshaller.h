//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSRPC2COMMUNICATION_UI_SETAPPICONMARSHALLER_INCLUDE
#define NSRPC2COMMUNICATION_UI_SETAPPICONMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/SetAppIcon.h"

namespace NsRPC2Communication
{
  namespace UI
  {

    struct SetAppIconMarshaller
    {
      static bool checkIntegrity(SetAppIcon& e);
      static bool checkIntegrityConst(const SetAppIcon& e);
    
      static bool fromString(const std::string& s,SetAppIcon& e);
      static const std::string toString(const SetAppIcon& e);
    
      static bool fromJSON(const Json::Value& s,SetAppIcon& e);
      static Json::Value toJSON(const SetAppIcon& e);
    };
  }
}

#endif
