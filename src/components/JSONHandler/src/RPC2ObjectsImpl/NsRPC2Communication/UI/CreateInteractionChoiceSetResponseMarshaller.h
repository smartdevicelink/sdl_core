//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSRPC2COMMUNICATION_UI_CREATEINTERACTIONCHOICESETRESPONSEMARSHALLER_INCLUDE
#define NSRPC2COMMUNICATION_UI_CREATEINTERACTIONCHOICESETRESPONSEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/CreateInteractionChoiceSetResponse.h"

namespace NsRPC2Communication
{
  namespace UI
  {

    struct CreateInteractionChoiceSetResponseMarshaller
    {
      static bool checkIntegrity(CreateInteractionChoiceSetResponse& e);
      static bool checkIntegrityConst(const CreateInteractionChoiceSetResponse& e);
    
      static bool fromString(const std::string& s,CreateInteractionChoiceSetResponse& e);
      static const std::string toString(const CreateInteractionChoiceSetResponse& e);
    
      static bool fromJSON(const Json::Value& s,CreateInteractionChoiceSetResponse& e);
      static Json::Value toJSON(const CreateInteractionChoiceSetResponse& e);
    };
  }
}

#endif
