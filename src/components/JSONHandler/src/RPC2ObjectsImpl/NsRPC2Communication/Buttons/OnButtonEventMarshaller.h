//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSRPC2COMMUNICATION_BUTTONS_ONBUTTONEVENTMARSHALLER_INCLUDE
#define NSRPC2COMMUNICATION_BUTTONS_ONBUTTONEVENTMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/Buttons/OnButtonEvent.h"

namespace NsRPC2Communication
{
  namespace Buttons
  {

    struct OnButtonEventMarshaller
    {
      static bool checkIntegrity(OnButtonEvent& e);
      static bool checkIntegrityConst(const OnButtonEvent& e);
    
      static bool fromString(const std::string& s,OnButtonEvent& e);
      static const std::string toString(const OnButtonEvent& e);
    
      static bool fromJSON(const Json::Value& s,OnButtonEvent& e);
      static Json::Value toJSON(const OnButtonEvent& e);
    };
  }
}

#endif
