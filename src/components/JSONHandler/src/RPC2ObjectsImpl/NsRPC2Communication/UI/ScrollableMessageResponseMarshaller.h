//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSRPC2COMMUNICATION_UI_SCROLLABLEMESSAGERESPONSEMARSHALLER_INCLUDE
#define NSRPC2COMMUNICATION_UI_SCROLLABLEMESSAGERESPONSEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/ScrollableMessageResponse.h"

namespace NsRPC2Communication
{
  namespace UI
  {

    struct ScrollableMessageResponseMarshaller
    {
      static bool checkIntegrity(ScrollableMessageResponse& e);
      static bool checkIntegrityConst(const ScrollableMessageResponse& e);
    
      static bool fromString(const std::string& s,ScrollableMessageResponse& e);
      static const std::string toString(const ScrollableMessageResponse& e);
    
      static bool fromJSON(const Json::Value& s,ScrollableMessageResponse& e);
      static Json::Value toJSON(const ScrollableMessageResponse& e);
    };
  }
}

#endif
