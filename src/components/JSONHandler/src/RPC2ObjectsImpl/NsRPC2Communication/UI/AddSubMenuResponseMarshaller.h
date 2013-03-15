//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSRPC2COMMUNICATION_UI_ADDSUBMENURESPONSEMARSHALLER_INCLUDE
#define NSRPC2COMMUNICATION_UI_ADDSUBMENURESPONSEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/AddSubMenuResponse.h"

namespace NsRPC2Communication
{
  namespace UI
  {

    struct AddSubMenuResponseMarshaller
    {
      static bool checkIntegrity(AddSubMenuResponse& e);
      static bool checkIntegrityConst(const AddSubMenuResponse& e);
    
      static bool fromString(const std::string& s,AddSubMenuResponse& e);
      static const std::string toString(const AddSubMenuResponse& e);
    
      static bool fromJSON(const Json::Value& s,AddSubMenuResponse& e);
      static Json::Value toJSON(const AddSubMenuResponse& e);
    };
  }
}

#endif
