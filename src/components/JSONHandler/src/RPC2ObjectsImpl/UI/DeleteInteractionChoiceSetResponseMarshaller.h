#ifndef RPC2COMMUNICATION_UI_DELETEINTERACTIONCHOICESETRESPONSEMARSHALLER_INCLUDE
#define RPC2COMMUNICATION_UI_DELETEINTERACTIONCHOICESETRESPONSEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/RPC2Objects/UI/DeleteInteractionChoiceSetResponse.h"

namespace RPC2Communication
{
  namespace UI
  {

    struct DeleteInteractionChoiceSetResponseMarshaller
    {
      static bool checkIntegrity(DeleteInteractionChoiceSetResponse& e);
      static bool checkIntegrityConst(const DeleteInteractionChoiceSetResponse& e);
    
      static bool fromString(const std::string& s,DeleteInteractionChoiceSetResponse& e);
      static const std::string toString(const DeleteInteractionChoiceSetResponse& e);
    
      static bool fromJSON(const Json::Value& s,DeleteInteractionChoiceSetResponse& e);
      static Json::Value toJSON(const DeleteInteractionChoiceSetResponse& e);
    };
  }
}

#endif
