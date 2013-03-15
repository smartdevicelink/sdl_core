//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSRPC2COMMUNICATION_SMARTDEVICELINKCORE_ONAPPDEACTIVATEDRESPONSEMARSHALLER_INCLUDE
#define NSRPC2COMMUNICATION_SMARTDEVICELINKCORE_ONAPPDEACTIVATEDRESPONSEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/BasicCommunication/OnAppDeactivatedResponse.h"

namespace NsRPC2Communication
{
  namespace BasicCommunication
  {

    struct OnAppDeactivatedResponseMarshaller
    {
      static bool checkIntegrity(OnAppDeactivatedResponse& e);
      static bool checkIntegrityConst(const OnAppDeactivatedResponse& e);
    
      static bool fromString(const std::string& s,OnAppDeactivatedResponse& e);
      static const std::string toString(const OnAppDeactivatedResponse& e);
    
      static bool fromJSON(const Json::Value& s,OnAppDeactivatedResponse& e);
      static Json::Value toJSON(const OnAppDeactivatedResponse& e);
    };
  }
}

#endif
