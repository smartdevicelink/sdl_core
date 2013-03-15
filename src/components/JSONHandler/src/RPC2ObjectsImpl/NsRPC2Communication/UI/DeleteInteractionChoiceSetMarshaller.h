//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSRPC2COMMUNICATION_UI_DELETEINTERACTIONCHOICESETMARSHALLER_INCLUDE
#define NSRPC2COMMUNICATION_UI_DELETEINTERACTIONCHOICESETMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/DeleteInteractionChoiceSet.h"

namespace NsRPC2Communication
{
  namespace UI
  {

    struct DeleteInteractionChoiceSetMarshaller
    {
      static bool checkIntegrity(DeleteInteractionChoiceSet& e);
      static bool checkIntegrityConst(const DeleteInteractionChoiceSet& e);
    
      static bool fromString(const std::string& s,DeleteInteractionChoiceSet& e);
      static const std::string toString(const DeleteInteractionChoiceSet& e);
    
      static bool fromJSON(const Json::Value& s,DeleteInteractionChoiceSet& e);
      static Json::Value toJSON(const DeleteInteractionChoiceSet& e);
    };
  }
}

#endif
