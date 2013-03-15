//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSRPC2COMMUNICATION_VR_CHANGEREGISTRATIONRESPONSEMARSHALLER_INCLUDE
#define NSRPC2COMMUNICATION_VR_CHANGEREGISTRATIONRESPONSEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/VR/ChangeRegistrationResponse.h"

namespace NsRPC2Communication
{
  namespace VR
  {

    struct ChangeRegistrationResponseMarshaller
    {
      static bool checkIntegrity(ChangeRegistrationResponse& e);
      static bool checkIntegrityConst(const ChangeRegistrationResponse& e);
    
      static bool fromString(const std::string& s,ChangeRegistrationResponse& e);
      static const std::string toString(const ChangeRegistrationResponse& e);
    
      static bool fromJSON(const Json::Value& s,ChangeRegistrationResponse& e);
      static Json::Value toJSON(const ChangeRegistrationResponse& e);
    };
  }
}

#endif
