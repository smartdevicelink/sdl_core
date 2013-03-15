//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSRPC2COMMUNICATION_UI_CHANGEREGISTRATIONMARSHALLER_INCLUDE
#define NSRPC2COMMUNICATION_UI_CHANGEREGISTRATIONMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/ChangeRegistration.h"

namespace NsRPC2Communication
{
  namespace UI
  {

    struct ChangeRegistrationMarshaller
    {
      static bool checkIntegrity(ChangeRegistration& e);
      static bool checkIntegrityConst(const ChangeRegistration& e);
    
      static bool fromString(const std::string& s,ChangeRegistration& e);
      static const std::string toString(const ChangeRegistration& e);
    
      static bool fromJSON(const Json::Value& s,ChangeRegistration& e);
      static Json::Value toJSON(const ChangeRegistration& e);
    };
  }
}

#endif
