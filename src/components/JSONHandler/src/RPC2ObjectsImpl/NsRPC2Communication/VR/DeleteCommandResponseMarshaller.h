#ifndef NSRPC2COMMUNICATION_VR_DELETECOMMANDRESPONSEMARSHALLER_INCLUDE
#define NSRPC2COMMUNICATION_VR_DELETECOMMANDRESPONSEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/VR/DeleteCommandResponse.h"

namespace NsRPC2Communication
{
  namespace VR
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
