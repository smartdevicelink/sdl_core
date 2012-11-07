#ifndef RPC2COMMUNICATION_UI_CREATEINTERACTIONCHOICESETRESPONSEMARSHALLER_INCLUDE
#define RPC2COMMUNICATION_UI_CREATEINTERACTIONCHOICESETRESPONSEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/RPC2Objects/UI/CreateInteractionChoiceSetResponse.h"

namespace RPC2Communication
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
