//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSRPC2COMMUNICATION_UI_SETDISPLAYLAYOUTMARSHALLER_INCLUDE
#define NSRPC2COMMUNICATION_UI_SETDISPLAYLAYOUTMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/SetDisplayLayout.h"

namespace NsRPC2Communication
{
  namespace UI
  {

    struct SetDisplayLayoutMarshaller
    {
      static bool checkIntegrity(SetDisplayLayout& e);
      static bool checkIntegrityConst(const SetDisplayLayout& e);
    
      static bool fromString(const std::string& s,SetDisplayLayout& e);
      static const std::string toString(const SetDisplayLayout& e);
    
      static bool fromJSON(const Json::Value& s,SetDisplayLayout& e);
      static Json::Value toJSON(const SetDisplayLayout& e);
    };
  }
}

#endif
