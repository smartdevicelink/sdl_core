//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSRPC2COMMUNICATION_VR_ONCOMMANDMARSHALLER_INCLUDE
#define NSRPC2COMMUNICATION_VR_ONCOMMANDMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/VR/OnCommand.h"

namespace NsRPC2Communication
{
  namespace VR
  {

    struct OnCommandMarshaller
    {
      static bool checkIntegrity(OnCommand& e);
      static bool checkIntegrityConst(const OnCommand& e);
    
      static bool fromString(const std::string& s,OnCommand& e);
      static const std::string toString(const OnCommand& e);
    
      static bool fromJSON(const Json::Value& s,OnCommand& e);
      static Json::Value toJSON(const OnCommand& e);
    };
  }
}

#endif
