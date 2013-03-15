//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSRPC2COMMUNICATION_UI_CREATEINTERACTIONCHOICESETMARSHALLER_INCLUDE
#define NSRPC2COMMUNICATION_UI_CREATEINTERACTIONCHOICESETMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/CreateInteractionChoiceSet.h"

namespace NsRPC2Communication
{
  namespace UI
  {

    struct CreateInteractionChoiceSetMarshaller
    {
      static bool checkIntegrity(CreateInteractionChoiceSet& e);
      static bool checkIntegrityConst(const CreateInteractionChoiceSet& e);
    
      static bool fromString(const std::string& s,CreateInteractionChoiceSet& e);
      static const std::string toString(const CreateInteractionChoiceSet& e);
    
      static bool fromJSON(const Json::Value& s,CreateInteractionChoiceSet& e);
      static Json::Value toJSON(const CreateInteractionChoiceSet& e);
    };
  }
}

#endif
