//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSRPC2COMMUNICATION_SMARTDEVICELINKCORE_ONAPPREGISTEREDMARSHALLER_INCLUDE
#define NSRPC2COMMUNICATION_SMARTDEVICELINKCORE_ONAPPREGISTEREDMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/BasicCommunication/OnAppRegistered.h"

namespace NsRPC2Communication
{
  namespace BasicCommunication
  {

    struct OnAppRegisteredMarshaller
    {
      static bool checkIntegrity(OnAppRegistered& e);
      static bool checkIntegrityConst(const OnAppRegistered& e);
    
      static bool fromString(const std::string& s,OnAppRegistered& e);
      static const std::string toString(const OnAppRegistered& e);
    
      static bool fromJSON(const Json::Value& s,OnAppRegistered& e);
      static Json::Value toJSON(const OnAppRegistered& e);
    };
  }
}

#endif
