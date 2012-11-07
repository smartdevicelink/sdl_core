#ifndef RPC2COMMUNICATION_UI_DELETECOMMANDRESPONSEMARSHALLER_INCLUDE
#define RPC2COMMUNICATION_UI_DELETECOMMANDRESPONSEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/RPC2Objects/UI/DeleteCommandResponse.h"

namespace RPC2Communication
{
  namespace UI
  {

    struct DeleteCommandResponseMarshaller
    {
      static bool checkIntegrity(DeleteCommandResponse& e);
      static bool checkIntegrityConst(const DeleteCommandResponse& e);
    
      static bool fromString(const std::string& s,DeleteCommandResponse& e);
      static const std::string toString(const DeleteCommandResponse& e);
    
      static bool fromJSON(const Json::Value& s,DeleteCommandResponse& e);
      static Json::Value toJSON(const DeleteCommandResponse& e);
    };
  }
}

#endif
