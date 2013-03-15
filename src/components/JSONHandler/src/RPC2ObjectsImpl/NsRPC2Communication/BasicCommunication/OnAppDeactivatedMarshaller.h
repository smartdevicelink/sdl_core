//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSRPC2COMMUNICATION_SMARTDEVICELINKCORE_ONAPPDEACTIVATEDMARSHALLER_INCLUDE
#define NSRPC2COMMUNICATION_SMARTDEVICELINKCORE_ONAPPDEACTIVATEDMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/BasicCommunication/OnAppDeactivated.h"

namespace NsRPC2Communication
{
  namespace BasicCommunication
  {

    struct OnAppDeactivatedMarshaller
    {
      static bool checkIntegrity(OnAppDeactivated& e);
      static bool checkIntegrityConst(const OnAppDeactivated& e);
    
      static bool fromString(const std::string& s,OnAppDeactivated& e);
      static const std::string toString(const OnAppDeactivated& e);
    
      static bool fromJSON(const Json::Value& s,OnAppDeactivated& e);
      static Json::Value toJSON(const OnAppDeactivated& e);
    };
  }
}

#endif
